package ru.nsu.nlyapich.client;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.config.Configurator;
import ru.nsu.nlyapich.connection.*;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

public class Client {
    private static final Logger logger = LogManager.getLogger(Client.class);
    private Connection connection;
    private static ModelClient model;
    private static GuiClient gui;
    private volatile boolean isConnect = false;

    public boolean isConnect() {
        return isConnect;
    }

    public void setConnect(boolean connect) {
        isConnect = connect;
    }

    public static void main(String[] args) {
        String configLoggerFile = Client.class.getClassLoader().getResource("log4j2.xml").getPath();
        Configurator.initialize(null, configLoggerFile);

        logger.info("Сlient creation");
        Client client = new Client();

        logger.info("Model creation");
        model = new ModelClient();

        logger.info("Gui creation");
        gui = new GuiClient(client);
        gui.initFrameClient();

        logger.info("Client running");
        while (true) {
            if (client.isConnect()) {
                client.nameUserRegistration();
                client.receiveMessageFromServer();
                client.setConnect(false);
            }
        }
    }

    protected void connectToServer(InetAddress address, int port, MessageFormat messageFormat) {
        if (!isConnect) {
            while (true) {
                try {
                    Socket socket = new Socket(address, port);
                    connection = ConnectionFactory.newConnection(messageFormat, socket);
                    isConnect = true;
                    gui.addMessage("Service message: You have connected to the server.\n");
                    break;
                } catch (Exception e) {
                    logger.error(e);
                    gui.errorDialogWindow("An error has occurred! The server may be down or the server address or port is incorrect. Try again or change the configuration file.");
                    break;
                }
            }
        } else {
            gui.errorDialogWindow("You are already connected.");
        }
    }

    protected void nameUserRegistration() {
        boolean flag = true;
        while (flag) {
            try {
                Message message = connection.receive();
                switch (message.getTypeMessage()){
                    case REQUEST_NAME_USER -> {
                        String nameUser = gui.getNameUser();
                        connection.send(new Message(MessageType.USER_NAME, nameUser));
                    }
                    case NAME_USED -> {
                        gui.errorDialogWindow("This name is already in use, please enter another one.");
                        String nameUser = gui.getNameUser();
                        connection.send(new Message(MessageType.USER_NAME, nameUser));
                    }
                    case NAME_ACCEPTED -> {
                        gui.addMessage("Service message: Your name has been accepted.\n");
                        model.setUsers(message.getListUsers());
                        flag = false;
                    }
                    default -> {
                    }
                }
            } catch (Exception e) {
                logger.error(e);
                gui.errorDialogWindow("An error occurred while registering the name. Try to reconnect.");
                try {
                    connection.close();
                    isConnect = false;
                    flag = false;
                } catch (IOException ex) {
                    logger.error(ex);
                    gui.errorDialogWindow("Error closing connection.");
                }
            }

        }
    }

    protected void sendMessageOnServer(String text) {
        try {
            connection.send(new Message(MessageType.TEXT_MESSAGE, text));
        } catch (Exception e) {
            logger.error(e);
            gui.errorDialogWindow("Error sending message.");
        }
    }

    protected void receiveMessageFromServer() {
        while (isConnect) {
            try {
                Message message = connection.receive();
                switch (message.getTypeMessage()) {
                    case TEXT_MESSAGE -> {
                        gui.addMessage(message.getTextMessage());
                    }
                    case USER_ADDED -> {
                        model.addUser(message.getTextMessage());
                        gui.refreshListUsers(model.getUsers());
                        gui.addMessage(String.format("Service message: User %s has joined the chat.\n", message.getTextMessage()));
                    }
                    case REMOVED_USER -> {
                        model.removeUser(message.getTextMessage());
                        gui.refreshListUsers(model.getUsers());
                        gui.addMessage(String.format("Service message: User %s has left the chat.\n", message.getTextMessage()));
                    }
                    default -> {
                    }
                }
            } catch (Exception e) {
                logger.error(e);
                gui.errorDialogWindow("An error occurred while receiving a message from the server.");
                setConnect(false);
                gui.refreshListUsers(model.getUsers());
                break;
            }
        }
    }

    protected void disableClient() {
        try {
            if (isConnect) {
                connection.send(new Message(MessageType.DISABLE_USER));
                model.getUsers().clear();
                isConnect = false;
                gui.refreshListUsers(model.getUsers());
            } else {
                gui.errorDialogWindow("You are already disabled.");
            }
        } catch (Exception e) {
            logger.error(e);
            gui.errorDialogWindow("Service message: An error occurred while disconnecting.");
        }
    }
}