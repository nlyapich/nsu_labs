package ru.nsu.nlyapich.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.config.Configurator;
import ru.nsu.nlyapich.connection.*;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class Server {
    private static final Logger logger = LogManager.getLogger(Server.class);
    private ServerSocket serverSocket;
    private static GuiServer gui;
    private static ModelServer model;
    private static volatile boolean isServerStart = false;
    private MessageFormat messageFormat;

    protected void startServer(InetAddress address, int port, MessageFormat messageFormat) {
        logger.info("Start server");
        try {
            serverSocket = new ServerSocket(port, 0, address);
            isServerStart = true;
            this.messageFormat = messageFormat;
            gui.refreshDialogWindowServer("The server is running.\n");
        } catch (Exception e) {
            logger.error(e);
            gui.refreshDialogWindowServer("Failed to start server.\n");
        }
    }

    protected void stopServer() {
        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                for (Map.Entry<String, Connection> user : model.getAllUsers().entrySet()) {
                    user.getValue().close();
                }
                serverSocket.close();
                model.getAllUsers().clear();
                gui.refreshDialogWindowServer("The server has been stopped.\n");
            } else {
                gui.refreshDialogWindowServer("The server is not running - there is nothing to stop!\n");
            }
        } catch (Exception e) {
            logger.error(e);
            gui.refreshDialogWindowServer("Failed to stop the server.\n");
        }
    }

    protected void acceptServer() {
        while (true) {
            try {
                Socket socket = serverSocket.accept();
                new ServerThread(socket).start();
            } catch (Exception e) {
                logger.error(e);
                gui.refreshDialogWindowServer("The connection to the server has been lost.\n");
                break;
            }
        }
    }

    protected void sendMessageAllUsers(Message message) {
        logger.info("Send message all users");
        for (Map.Entry<String, Connection> user : model.getAllUsers().entrySet()) {
            try {
                user.getValue().send(message);
            } catch (Exception e) {
                logger.error(e);
                gui.refreshDialogWindowServer("Error sending message to all users.\n");
            }
        }
    }

    public static void main(String[] args) {
        String configLoggerFile = Server.class.getClassLoader().getResource("log4j2.xml").getPath();
        Configurator.initialize(null, configLoggerFile);

        logger.info("Server creation");
        Server server = new Server();
        logger.info("Gui creation");
        gui = new GuiServer(server);
        logger.info("Model creation");
        model = new ModelServer();
        gui.initFrameServer();
        while (true) {
            if (isServerStart) {
                server.acceptServer();
                isServerStart = false;
            }
        }
    }

    private class ServerThread extends Thread {
        private static final Logger logger = LogManager.getLogger(ServerThread.class);
        private Socket socket;

        public ServerThread(Socket socket) {
            logger.info("ServerThread creation");
            this.socket = socket;
        }

        private boolean isCorrectName(String userName){
            return (userName != null && !userName.isEmpty() && !model.getAllUsers().containsKey(userName));
        }

        private String requestAndAddingUser(Connection connection) throws IOException {
            logger.info("Request and adding user");
            for (int i = 0; i < 10; ++i) {
                try {
                    connection.send(new Message(MessageType.REQUEST_NAME_USER));
                    Message responseMessage = connection.receive();
                    String userName = responseMessage.getTextMessage();
                    if (responseMessage.getTypeMessage() == MessageType.USER_NAME && isCorrectName(userName)) {
                        model.addUser(userName, connection);
                        Set<String> listUsers = new HashSet<>();
                        for (Map.Entry<String, Connection> users : model.getAllUsers().entrySet()) {
                            listUsers.add(users.getKey());
                        }
                        connection.send(new Message(MessageType.NAME_ACCEPTED, listUsers));
                        sendMessageAllUsers(new Message(MessageType.USER_ADDED, userName));
                        return userName;
                    }
                    else {
                        connection.send(new Message(MessageType.NAME_USED));
                    }
                } catch (Exception e) {
                    logger.error(e);
                    gui.refreshDialogWindowServer("An error occurred while requesting and adding a new user.\n");
                }
            }
            connection.close();
            logger.error("Failed to add new user");
            gui.refreshDialogWindowServer("Failed to add new user.\n");
            return null;
        }

        private void messagingBetweenUsers(Connection connection, String userName) {
            boolean flag = true;
            while (flag) {
                try {
                    Message message = connection.receive();
                    switch (message.getTypeMessage()){
                        case TEXT_MESSAGE -> {
                            String textMessage = String.format("%s: %s\n", userName, message.getTextMessage());
                            sendMessageAllUsers(new Message(MessageType.TEXT_MESSAGE, textMessage));
                        }
                        case DISABLE_USER -> {
                            sendMessageAllUsers(new Message(MessageType.REMOVED_USER, userName));
                            model.removeUser(userName);
                            connection.close();
                            gui.refreshDialogWindowServer(String.format("User %s has logged out.\n", socket.getRemoteSocketAddress()));
                            flag = false;
                        }
                        default -> {
                        }
                    }
                } catch (Exception e) {
                    logger.error(e);
                    gui.refreshDialogWindowServer(String.format("An error occurred while sending a message from user %s, or disconnected.\n", userName));
                    flag = false;
                }
            }
        }

        @Override
        public void run() {
            gui.refreshDialogWindowServer(String.format("A new user has connected with a remote socket - %s.\n", socket.getRemoteSocketAddress()));
            try {
                Connection connection = ConnectionFactory.newConnection(messageFormat, socket);
                String nameUser = requestAndAddingUser(connection);
                if (nameUser == null){
                    return;
                }
                messagingBetweenUsers(connection, nameUser);
            } catch (Exception e) {
                logger.error(e);
                gui.refreshDialogWindowServer(String.format("An error occurred while sending a message from the user.\n"));
            }
        }
    }
}
