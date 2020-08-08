#pragma once

class Singleton
{
public:
    static Singleton& instance();
    static void init();
    static void dispose();

private:
    static Singleton* _instance;
};
