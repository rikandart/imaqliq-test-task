#include <iostream>
#include <client.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ClientTest, SockConnected){
    Client cli(23457);
    EXPECT_TRUE(cli.connected());
}

TEST(ClientTest, FileName){
    Client cli(23457);
    cli.setFileName("filename");
    EXPECT_EQ(cli.fileName().compare("filename"), 0);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}