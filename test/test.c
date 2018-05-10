#include <stdint.h>
#include <unistd.h>
#include <security/pam_modules.h>
#include "../src/pam_panic/pam_panic_authdevice.h"
#include "../src/pam_panic/pam_panic_reject.h"
#include <CUnit/Basic.h>


#define STATE_GOOD 0
#define STATE_BAD 99
#define STATE_NA 1

#define STATE_REJ_SER 0
#define STATE_REJ_REB 1
#define STATE_REJ_POW 2
#define STATE_REJ_NA 3

#define GOODUUID "./good"
#define BADUUID  "./bad"

char* gU = GOODUUID;
char* bU = BADUUID;

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}


// pam_panic_authdevice tests
void test_authDeviceGood(void) {
  FILE *f = fopen(gU, "w");
  fclose(f);

  int ret = authDevice(NULL, gU, bU, NULL, 0, 0, 0);
  CU_ASSERT_EQUAL(ret, STATE_GOOD);
  unlink(gU);
}

void test_authDeviceBad(void) {
  FILE *f = fopen(bU, "w");
  fclose(f);
  
  int ret = authDevice(NULL, gU, bU, NULL, 0, 0, 0);
  CU_ASSERT_EQUAL(ret, STATE_BAD);
  unlink(bU);
}

void test_authDeviceNA(void) {
  int ret = authDevice(NULL, gU, bU, NULL, 0, 0, 0);
  CU_ASSERT_EQUAL(ret, STATE_NA);
}


// pam_panic_reject tests
void test_rejectSerious(void) {
  int ret = reject(NULL, 1, 0, 0);
  CU_ASSERT_EQUAL(ret, STATE_REJ_SER); 
}

void test_rejectReboot(void) {
  int ret = reject(NULL, 0, 1, 0);
  CU_ASSERT_EQUAL(ret, STATE_REJ_REB);
}

void test_rejectPoweroff(void) {
  int ret = reject(NULL, 0, 0, 1);
  CU_ASSERT_EQUAL(ret, STATE_REJ_POW);
}

void test_rejectNA(void) {
  int ret = reject(NULL, 0, 0, 0);
  CU_ASSERT_EQUAL(ret, STATE_REJ_NA);
}



int main(void) {

  // no stdout buffering
  setbuf(stdout, NULL);
  
  // init CUnit test registry
  CU_pSuite pSuiteDevice = NULL;
  CU_pSuite pSuiteReject = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  // Make suits
  pSuiteDevice = CU_add_suite("Suite pam_panic_authdevice", init_suite, clean_suite);
  pSuiteReject = CU_add_suite("Suite pam_panic_reject", init_suite, clean_suite);
  if (pSuiteDevice == NULL
    || pSuiteReject == NULL) { 
    CU_cleanup_registry();
    return CU_get_error();
  }

  // adding tests to all suits
          // SuiteDevice
  if (   (NULL == CU_add_test(pSuiteDevice, "Authenticate with good device?", test_authDeviceGood))
      || (NULL == CU_add_test(pSuiteDevice, "Authenticate with bad device?", test_authDeviceBad))
      || (NULL == CU_add_test(pSuiteDevice, "Authenticate with no device?", test_authDeviceNA))
      || (NULL == CU_add_test(pSuiteReject, "Reject: Serious?", test_rejectSerious))
      || (NULL == CU_add_test(pSuiteReject, "Reject: Reboot?", test_rejectReboot))
      || (NULL == CU_add_test(pSuiteReject, "Reject: Poweroff?", test_rejectPoweroff))
      || (NULL == CU_add_test(pSuiteReject, "Reject: Nothing?", test_rejectNA))
     ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  return 0;
}

