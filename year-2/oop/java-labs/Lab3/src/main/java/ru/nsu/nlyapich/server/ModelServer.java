package ru.nsu.nlyapich.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.connection.Connection;

import java.util.HashMap;
import java.util.Map;

public class ModelServer {
    private static final Logger logger = LogManager.getLogger(ModelServer.class);
    private Map<String, Connection> allUsers = new HashMap<>();

    protected Map<String, Connection> getAllUsers() {
        logger.debug("Get all users");
        return allUsers;
    }

    protected void addUser(String nameUser, Connection connection) {
        logger.info("Add new user");
        allUsers.put(nameUser, connection);
    }

    protected void removeUser(String nameUser) {
        logger.info("Remove user");
        allUsers.remove(nameUser);
    }

}
