package org.example.server;

import java.io.IOException;

public class Main {

    public static void main(String[] args) {

        int listeningPort = 9999;

        try {
            Thread listenerThread = new Thread(new ListenerTask(listeningPort));
            listenerThread.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
