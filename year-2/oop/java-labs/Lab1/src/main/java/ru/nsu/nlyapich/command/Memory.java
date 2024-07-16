package ru.nsu.nlyapich.command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Memory {

    private static final Logger logger = LogManager.getLogger(Memory.class);
    private int _pointer;
    private int[] _data;

    public Memory(int size) {
        logger.info("Creating Memory");
        _pointer = 0;
        _data = new int[size];
    }

    public int getPointer() {
        return _pointer;
    }

    public int getCurrentData() {
        return _data[_pointer];
    }

    public void setCurrentData(int data) { _data[_pointer] = data;}

    public void incrementPointer() {
        if (_pointer == _data.length) {
            logger.error("Out of range");
            throw new IndexOutOfBoundsException();
        }
        _pointer++;
    }

    public void decrementPointer() {
        if (_pointer == 0) {
            logger.error("Out of range");
            throw new IndexOutOfBoundsException();
        }
        _pointer--;
    }

    public void incrementData() {
        _data[_pointer]++;
    }

    public void decrementData() {
        _data[_pointer]--;
    }
}
