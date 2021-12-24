#include <iostream>
#include <server.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ServerTest, SockBinded){
    Server srv(23457);
    EXPECT_TRUE(srv.binded());
}

TEST(ServerTest, FileName){
    Server srv(23457);
    srv.setFileName("filename");
    EXPECT_EQ(srv.fileName().compare("filename"), 0);
}

TEST(ServerTest, Save){
    Server srv(23457);
    srv.setFileName("test_fn");
    EXPECT_EQ(srv.fileName().compare("test_fn"), 0);
    EXPECT_TRUE(srv.listenAndSave());
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}