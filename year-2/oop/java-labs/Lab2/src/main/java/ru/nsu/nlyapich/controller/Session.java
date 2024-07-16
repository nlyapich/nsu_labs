package ru.nsu.nlyapich.controller;

public interface Session {
    boolean isSwitchStatus();
    void updateSwitchStatus();
    StatusSession getStatus();
}
