#include "BOSS.h"

using namespace BOSS;

GameState getInitialState(const RaceID race)
{
    GameState initialState(race);
    initialState.setStartingState();

    if (false)//race == Races::Protoss)
    {
        initialState.doAction(ActionTypes::GetActionType("Protoss_Probe"));
        initialState.doAction(ActionTypes::GetActionType("Protoss_Probe"));
        initialState.doAction(ActionTypes::GetActionType("Protoss_Probe"));
        initialState.doAction(ActionTypes::GetActionType("Protoss_Probe"));
        initialState.doAction(ActionTypes::GetActionType("Protoss_Pylon"));
    }

    return initialState;
}

ActionSet getRelevantActions(const RaceID race)
{
    ActionSet actions;

    if (race == Races::Protoss)
    {
        actions.add(ActionTypes::GetActionType("Protoss_Probe"));
        actions.add(ActionTypes::GetActionType("Protoss_Pylon"));
        actions.add(ActionTypes::GetActionType("Protoss_Nexus"));
        actions.add(ActionTypes::GetActionType("Protoss_Assimilator"));
        actions.add(ActionTypes::GetActionType("Protoss_Zealot"));
        actions.add(ActionTypes::GetActionType("Protoss_Gateway"));
        actions.add(ActionTypes::GetActionType("Protoss_Dragoon"));
        actions.add(ActionTypes::GetActionType("Protoss_Cybernetics_Core"));
    }

    return actions;
}

CombatSearchParameters getParams(const RaceID race)
{
    CombatSearchParameters params;

    if (race == Races::Protoss)
    {
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Probe"), 12);
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Nexus"), 1);
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Assimilator"), 1);
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Cybernetics_Core"), 1);
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Pylon"), 4);
        params.setMaxActions(ActionTypes::GetActionType("Protoss_Gateway"), 3);

        params.setRelevantActions(getRelevantActions(Races::Protoss));
    }

    return params;
}

void testCombatSearch()
{
    RaceID race = Races::Protoss;
    CombatSearchParameters params = getParams(race);
    params.setInitialState(getInitialState(race));
    params.setFrameTimeLimit(3600);

    //params.setSearchTimeLimit(100);
    
    Timer t;
    t.start();

    CombatSearch tcs(params);
    tcs.search();

    double ms = t.getElapsedTimeInMilliSec();
    std::cout << "\nNormal " << tcs.getResults().nodesExpanded << " nodes searched in " << ms << "ms (" << (unsigned long long)(tcs.getResults().nodesExpanded / ms) << "k nodes/s)\n\n";

    for (size_t a(0); a<tcs.getResults().buildOrder.size(); ++a)
    {
        std::cout << tcs.getResults().buildOrder[a].getName() << "\n";
    }
}

void testNaiveBuildOrder()
{
    GameState initialState(Races::Protoss);
    initialState.setStartingState();
    GameState initialStateNew(initialState);
    DFBB_BuildOrderSearchGoal goal(initialState.getRace());

    goal.setGoal(ActionTypes::GetActionType("Protoss_Zealot"), 3);

    Timer t;
    t.start();
    std::vector<ActionType> buildOrder = Tools::GetNaiveBuildOrder(initialStateNew, goal);
    double ms = t.getElapsedTimeInMilliSec();

    Timer tnew;
    tnew.start();
    std::vector<ActionType> buildOrderNew = Tools::GetOptimizedNaiveBuildOrder(initialStateNew, goal);
    double msnew = tnew.getElapsedTimeInMilliSec();

    for (size_t i(0); i<buildOrder.size(); ++i)
    {
        std::cout << buildOrder[i].getName() << std::endl;
        initialState.doAction(buildOrder[i]);
    }

    std::cout << "Build order took " << initialState.getLastActionFinishTime() << " frames to complete in " << ms << " ms\n\n" << std::endl;

    for (size_t i(0); i<buildOrderNew.size(); ++i)
    {
        std::cout << buildOrderNew[i].getName() << std::endl;
        initialStateNew.doAction(buildOrderNew[i]);
    }

    std::cout << "New build order took " << initialStateNew.getLastActionFinishTime() << " frames to complete with " << initialStateNew.getUnitData().getNumTotal(ActionTypes::GetWorker(initialStateNew.getRace())) << " total workers in " << msnew << " ms" << std::endl;
}

void testBuildOrderVisualization()
{
    const RaceID race = Races::Zerg;

    GameState state(race);
    state.setStartingState();

    std::vector<ActionType> buildOrder;

    if (race == Races::Protoss)
    {
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Pylon"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Assimilator"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Gateway"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Probe"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Zealot"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Cybernetics_Core"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Pylon"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Pylon"));
        buildOrder.push_back(ActionTypes::GetActionType("Protoss_Pylon"));
    }
    else if (race == Races::Zerg)
    {
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Hatchery"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Spawning_Pool"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Hatchery"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Extractor"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Lair"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Extractor"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Zergling"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Zergling"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Zergling"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Spire"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));

        /*buildOrder.push_back(ActionTypes::GetActionType("Zerg_Hatchery"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Extractor"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Spawning_Pool"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Lair"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Drone"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Spire"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Overlord"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));
        buildOrder.push_back(ActionTypes::GetActionType("Zerg_Mutalisk"));*/
    }
    
    BOSS::GUI::Instance().OnStart();
    BOSS::GUI::Instance().SetState(state);
    
    size_t nextActionIndex = 0;
    while (true)
    {
        BOSS::GUI::Instance().OnFrame();

        if (nextActionIndex < buildOrder.size())
        {
            FrameCountType nextActionFrame = state.whenCanPerform(buildOrder[nextActionIndex]);

            if (nextActionFrame == state.getCurrentFrame())
            {
                state.doAction(buildOrder[nextActionIndex]);
                std::cout << state.getCurrentFrame() << " Action Performed: " << buildOrder[nextActionIndex].getName() << std::endl;
                nextActionIndex++;
            }
        }

        state.fastForward(state.getCurrentFrame() + 1);
        BOSS::GUI::Instance().SetState(state);
        BOSS::GUI::Instance().SetBuildOrder(buildOrder, nextActionIndex);
        
        Timer t;
        t.start();
        while (t.getElapsedTimeInMilliSec() < 5) {}
    }
}

int main(int argc, char *argv[])
{
    #ifdef BOSS_ENABLE_GUI
        glutInit(&argc, argv);
    #endif

    BOSS::init();
    int a;
    GameState initialState(Races::Protoss);
    initialState.setStartingState();

    testBuildOrderVisualization();

    //testNaiveBuildOrder();

    DFBB_BuildOrderSmartSearch smartSearch(initialState.getRace());
    smartSearch.setState(initialState);
    smartSearch.addGoal(ActionTypes::GetActionType("Protoss_Zealot"), 6);
    smartSearch.getParameters().goal.printGoal();
    smartSearch.setTimeLimit(50000);
    
    Timer t;
    t.start();
    while (!smartSearch.getResults().solved)
    {
        smartSearch.search();
    }

    double ms = t.getElapsedTimeInMilliSec();

    smartSearch.getResults().printResults(true);
    std::cout << std::endl << "Search Took: " << ms << "ms" << std::endl;

    //Tools::TestBuildOrderUpperBound();

    //testCombatSearch();
    //testThreadCombatSearch();

    return 0;
}

