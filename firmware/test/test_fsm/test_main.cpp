// Host-native logic tests for MinerPodFSM. Run with `pio test -e native`
// -- no hardware required.
#include <unity.h>
#include "MinerPodFSM.h"

void setUp(void) {}
void tearDown(void) {}

static void test_idle_to_anchoring_on_dispatch(void) {
  MinerPodFSM fsm;
  fsm.begin(0);
  TEST_ASSERT_EQUAL(Idle, fsm.state());

  MinerPodSensors s;
  fsm.update(s, 10);
  TEST_ASSERT_EQUAL(Idle, fsm.state());

  s.dispatchReceived = true;
  fsm.update(s, 20);
  TEST_ASSERT_EQUAL(Anchoring, fsm.state());
}

static void test_full_happy_path(void) {
  MinerPodFSM fsm;
  fsm.begin(0);
  MinerPodSensors s;

  s.dispatchReceived = true;
  fsm.update(s, 0);
  TEST_ASSERT_EQUAL(Anchoring, fsm.state());

  s.anchorConfirmed = true;
  fsm.update(s, 10);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());

  // Only one of three seal sensors -- should NOT be enough to proceed.
  s.sealSensor1 = true;
  fsm.update(s, 20);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());

  // Two of three -- 2-of-3 voting passes.
  s.sealSensor2 = true;
  fsm.update(s, 30);
  TEST_ASSERT_EQUAL(Drilling, fsm.state());

  s.materialCollected = true;
  fsm.update(s, 40);
  TEST_ASSERT_EQUAL(Crushing, fsm.state());

  s.crushComplete = true;
  fsm.update(s, 50);
  TEST_ASSERT_EQUAL(Refining, fsm.state());

  s.refineComplete = true;
  fsm.update(s, 60);
  TEST_ASSERT_EQUAL(Transfer, fsm.state());

  s.transferConfirmed = true;
  fsm.update(s, 70);
  TEST_ASSERT_EQUAL(Idle, fsm.state());
}

static void test_single_seal_sensor_never_gates_drilling_alone(void) {
  MinerPodFSM fsm;
  fsm.begin(0);
  MinerPodSensors s;
  s.dispatchReceived = true;
  fsm.update(s, 0);
  s.anchorConfirmed = true;
  fsm.update(s, 10);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());

  // Cycle through each single sensor alone -- none should be sufficient.
  s.sealSensor1 = true;
  fsm.update(s, 20);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());
  s.sealSensor1 = false;

  s.sealSensor2 = true;
  fsm.update(s, 30);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());
  s.sealSensor2 = false;

  s.sealSensor3 = true;
  fsm.update(s, 40);
  TEST_ASSERT_EQUAL(ContainmentDeploy, fsm.state());
}

static void test_fault_preempts_and_is_sticky(void) {
  MinerPodFSM fsm;
  fsm.begin(0);
  MinerPodSensors s;
  s.dispatchReceived = true;
  fsm.update(s, 0);
  TEST_ASSERT_EQUAL(Anchoring, fsm.state());

  s.faultDetected = true;
  fsm.update(s, 10);
  TEST_ASSERT_EQUAL(Fault, fsm.state());

  s.faultDetected = false;
  s.anchorConfirmed = true;
  fsm.update(s, 20);
  TEST_ASSERT_EQUAL(Fault, fsm.state());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_idle_to_anchoring_on_dispatch);
  RUN_TEST(test_full_happy_path);
  RUN_TEST(test_single_seal_sensor_never_gates_drilling_alone);
  RUN_TEST(test_fault_preempts_and_is_sticky);
  return UNITY_END();
}
