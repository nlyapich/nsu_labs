package ru.nsu.nlyapich.client;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.HashSet;
import java.util.Set;

public class ModelClient {
    private static final Logger logger = LogManager.getLogger(ModelClient.class);
    private Set<String> users = new HashSet<>();

    protected Set<String> getUsers() {
        logger.debug("Get users");
        return users;
    }

    protected void addUser(String nameUser) {
        logger.info("Add new user");
        users.add(nameUser);
    }

    protected void removeUser(String nameUser) {
        logger.info("Remove user");
        users.remove(nameUser);
    }

    protected void setUsers(Set<String> users) {
        logger.debug("Set users");
        this.users = users;
    }
}
