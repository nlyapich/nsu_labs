package ru.nsu.nlyapich.connection;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.xml.bind.annotation.*;
import java.io.Serializable;
import java.util.Set;

@XmlAccessorType(XmlAccessType.FIELD)
@XmlRootElement
public class Message implements Serializable {
    private static final Logger logger = LogManager.getLogger(Message.class);
    @XmlAttribute
    private MessageType typeMessage;
    @XmlElement
    private String textMessage;
    @XmlElementWrapper(name = "users") @XmlElement(name = "user")
    private Set<String> listUsers;

    public Message() {
        textMessage = null;
        typeMessage = null;
        listUsers = null;
    }

    public Message(MessageType typeMessage, String textMessage) {
        logger.debug("Creation message");
        this.textMessage = textMessage;
        this.typeMessage = typeMessage;
        this.listUsers = null;
    }

    public Message(MessageType typeMessage, Set<String> listUsers) {
        logger.debug("Creation message");
        this.typeMessage = typeMessage;
        this.textMessage = null;
        this.listUsers = listUsers;
    }

    public Message(MessageType typeMessage) {
        logger.debug("Creation message");
        this.typeMessage = typeMessage;
        this.textMessage = null;
        this.listUsers = null;
    }

    public MessageType getTypeMessage() {
        logger.debug("Get type message");
        return typeMessage;
    }

    public Set<String> getListUsers() {
        logger.debug("Get list users");
        return listUsers;
    }

    public String getTextMessage() {
        logger.debug("Get text message");
        return textMessage;
    }

}
