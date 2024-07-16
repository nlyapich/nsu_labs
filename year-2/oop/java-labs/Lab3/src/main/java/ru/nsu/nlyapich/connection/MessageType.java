package ru.nsu.nlyapich.connection;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;

@XmlAccessorType(XmlAccessType.FIELD)
public enum MessageType {
    REQUEST_NAME_USER,
    TEXT_MESSAGE,
    NAME_ACCEPTED,
    USER_NAME,
    NAME_USED,
    USER_ADDED,
    DISABLE_USER,
    REMOVED_USER;
}
