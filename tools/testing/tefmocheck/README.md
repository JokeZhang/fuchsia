# Tefmocheck

Tefmocheck (Testing Failure Mode Checker) analyzes the outputs of a testing
Swarming task and determines whether various failures modes occured. It produces
a testing summary file (summmary.json) that contains a synthetic test for each
failure mode starting with "testing_failure_mode/".

At most a single check (the most specific one possible) will fail on a single
task. The least specific ones start with "testing_failure_mode/task_status/".
This just surfaces the Swarming task status. To see further details, see the
infra_and_test_std_and_klog.txt, which includes the output of the Swarming task.
