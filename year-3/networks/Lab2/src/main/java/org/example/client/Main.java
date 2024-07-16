package org.example.client;

public class Main {

    public static void main(String[] args) {
        String filePath = "/home/n_lyapich/Загрузки/graph.png";
        String serverName = "172.20.10.5";
        String serverPort = "9999";

        int port = 9999;
        try {
//            port = Integer.parseInt(serverPort);
        } catch (NumberFormatException e) {
            System.err.println("INVALID SERVER PORT");
            System.exit(1);
        }

        SendService.sendFile(filePath, serverName, port);
    }
}
