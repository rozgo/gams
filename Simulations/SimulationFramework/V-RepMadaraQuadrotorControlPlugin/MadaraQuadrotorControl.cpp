/*********************************************************************
* Usage of this software requires acceptance of the SMASH-CMU License,
* which can be found at the following URL:
*
* https://code.google.com/p/smash-cmu/wiki/License
*********************************************************************/
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "MadaraQuadrotorControl.h"
#include "utilities/Position.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

#ifdef WIN_VREP
    // Only include the custom transport in Windows, as it is not necessary in Linux.
    #include "Windows_Multicast_Transport.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor, sets up a Madara knowledge base and basic values.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
MadaraQuadrotorControl::MadaraQuadrotorControl(int droneId)
{
    // Initializes this to one, as for now we have only one drone using this controller.
    numDrones = 1;

    // Control id is derived from the droneId. But it has to be different to it to ensure different ids inside this domain.
    int transportId = droneId + 100;

    // Define the transport settings.
    Madara::Transport::Settings transportSettings;
    transportSettings.hosts_.resize(1);
    transportSettings.hosts_[0] = SIMULATED_HW_MULTICAST_ADDRESS;
    transportSettings.id = transportId;
    transportSettings.domains = VREP_DOMAIN;
    transportSettings.queue_length = SIMULATION_TRANSPORT_QUEUE_LENGTH;

    // Setup the actual transport.
#ifndef WIN_VREP
    // In Linux, or Windows outside of V-Rep, we can use the default Mulitcast transport.
    transportSettings.type = Madara::Transport::MULTICAST;
#endif
    
    // Delay launching the transport so that we can activate it when we want to, and setup logging.
    transportSettings.delay_launch = true;
    Madara::Knowledge_Engine::Knowledge_Base::log_level (10);
    Madara::Knowledge_Engine::Knowledge_Base::log_to_file(
      "quadrotormadaralog.txt", true);

    // Create the knowledge base.
    m_knowledge = new Madara::Knowledge_Engine::Knowledge_Base("", transportSettings);
    Madara::Knowledge_Record::set_precision(10);
    m_knowledge->print ("Past the Knowledge_Base creation.\n");

#ifdef WIN_VREP
    // In Windows with V-Rep we need a custom transport to avoid crashes due to incompatibilities between Win V-Rep and ACE.
    m_knowledge->attach_transport(new Windows_Multicast_Transport (m_knowledge->get_id (),
                                  m_knowledge->get_context (), transportSettings, true));
#else
    // Everywhere else we just activate the default Multicast transport.
    m_knowledge->activate_transport ();
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializes Madara variables for communicating with the drone platform.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraQuadrotorControl::initInternalData(int droneId)
{
    // Initialize the internal command variable so that we start with no commands.
    string droneIdString = std::to_string(static_cast<long long>(droneId));
    clearCommand(droneIdString);

    // Indicate that we have not received or replied to commands yet.
    m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MS_SIM_CMD_SENT_ID, (Madara::Knowledge_Record::Integer) 0,
                Madara::Knowledge_Engine::Eval_Settings(true, true));
    m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MS_SIM_CMD_RCVD_ID, (Madara::Knowledge_Record::Integer) 0,
                Madara::Knowledge_Engine::Eval_Settings(true, true));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructor, simply cleans up.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
MadaraQuadrotorControl::~MadaraQuadrotorControl()
{    
    terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cleanup, terminating all threads and open communications.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MadaraQuadrotorControl::terminate()
{
    if(m_knowledge != NULL)
    {
        bool otherDronesStillUsingPlugin = numDrones > 0;
        if(otherDronesStillUsingPlugin)
        {
            // If there are other simulated drones still using the plugin, we won't terminate and cleanup yet.
            MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
              DLINFO "MadaraQuadrotorControl::terminate:" \
              "Not terminating knowledge base, controller still in use by other %d drones.\n", numDrones));
        }
        else
        {
            // If there are no more drones using the plugin, we cleanup the knowledge base.
            MADARA_DEBUG (MADARA_LOG_MAJOR_EVENT, (LM_DEBUG, 
              DLINFO "MadaraQuadrotorControl::terminate:" \
              "Terminating Madara knowledge base.\n"));

            //m_knowledge->close_transport();
            m_knowledge->clear();
            delete m_knowledge;
            m_knowledge = NULL;
        
            return true;
        }
    }

    // We didn't really do any cleanup if we got here.
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraQuadrotorControl::updateQuadrotorPosition(const int& id, const double& lat,
    const double& lon, const double& z) // need to update for altitude
{
    // update the location of this drone (this would be done by its sensors).
    string droneIdString = NUM_TO_STR(id);
    if(m_knowledge != NULL)
    {
        m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MV_LATITUDE, (lat),
            Madara::Knowledge_Engine::Eval_Settings(true));
        m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MV_LONGITUDE, (lon),
            Madara::Knowledge_Engine::Eval_Settings(true));
        m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MV_ALTITUDE, (z));

        m_knowledge->print_knowledge(1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Updates the status of the drones in Madara.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraQuadrotorControl::updateQuadrotorStatus(const Status& s)
{
    // Need to update for altitude.
    updateQuadrotorPosition(s.m_id, s.m_loc.m_lat, s.m_loc.m_long, s.m_loc.m_alt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets a target position where a drone should move to.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
MadaraQuadrotorControl::Command* MadaraQuadrotorControl::getNewCommand(int droneId)
{
    string droneIdString = std::to_string(static_cast<long long>(droneId));

    // Check if commands have started being received for the requested drone.
    int recievedCommandId = (int) m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MS_SIM_CMD_SENT_ID).to_integer();
    if(recievedCommandId == 0)
        return NULL;
    
    // Get the actual command, if there is a new one.
    string commandStr = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_REQUESTED).to_string();
    if(commandStr == "0")
        return NULL;

    // Create the movement command to return.
    Command* command = new Command();
    command->m_command = commandStr;

    // Depending on the command, we may need to get more parameters.
    if(commandStr == MO_MOVE_TO_GPS_CMD)
    {
        // Move to certain location command; get the target location.
        double targetPosLat = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_CMD_ARG("0")).to_double();
        double targetPosLon = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_CMD_ARG("1")).to_double();

        // We don't care about alt here.
        Location targetLocation = Location(targetPosLat, targetPosLon, 0);
        command->m_loc = targetLocation;
    }
    else if(commandStr == MO_MOVE_TO_ALTITUDE_CMD)
    {
        double targetAltitude = m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_CMD_ARG("0")).to_double();

        // We don't care about lat and long here.
        Location targetLocation = Location(0, 0, targetAltitude);
        command->m_loc = targetLocation;
    }

    // Set the command as 0 locally, to indicate that we already read it.
    clearCommand(droneIdString);

    // Indicate that this is the last command we have received.
    int lastSentCmdId = (int) m_knowledge->get(MS_SIM_DEVICES_PREFIX + droneIdString + MS_SIM_CMD_SENT_ID).to_integer();
    m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MS_SIM_CMD_RCVD_ID, (Madara::Knowledge_Record::Integer) lastSentCmdId,
        Madara::Knowledge_Engine::Eval_Settings(false, false));

    return command;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set the command as 0 locally, to indicate that we already read it.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraQuadrotorControl::clearCommand(std::string droneIdString)
{
    m_knowledge->set(MS_SIM_DEVICES_PREFIX + droneIdString + MV_MOVEMENT_REQUESTED, "0",
        Madara::Knowledge_Engine::Eval_Settings(true, true));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MadaraQuadrotorControl::setNewThermalScan(int droneId, std::string thermalBuffer, int thermalRows, int thermalColumns)
{
    if(m_knowledge != NULL)
    {
        std::stringstream thermalBufferName;
        thermalBufferName << MS_SIM_DEVICES_PREFIX << droneId << MV_THERMAL_BUFFER;
        m_knowledge->set(thermalBufferName.str(), thermalBuffer);
    }
}
