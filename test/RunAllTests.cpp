#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Sparring);
IMPORT_TEST_GROUP(Impact);
IMPORT_TEST_GROUP(Peak);
IMPORT_TEST_GROUP(MaxQueue);
IMPORT_TEST_GROUP(MeasureAndDisplayByMode);
IMPORT_TEST_GROUP(Display);
IMPORT_TEST_GROUP(Calculate);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}