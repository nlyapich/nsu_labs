package ru.nsu.nlyapich.model;

public class Player extends Mobe {
    public double rotationAngle;
    public Player(double x, double y, double rotationAngle, int power, int maxHealth){
        super(x, y, power, maxHealth);
        this.rotationAngle = rotationAngle;
    }

    public int getMaxHealth(){
        return maxHealth;
    }

    public int getCurHealth(){
        return curHealth;
    }


}
