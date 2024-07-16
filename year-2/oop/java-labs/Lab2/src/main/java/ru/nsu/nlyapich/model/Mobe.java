package ru.nsu.nlyapich.model;

public class Mobe {
    public double x;
    public double y;
    protected int power;
    protected int maxHealth;
    protected int curHealth;
    public Mobe(double x, double y, int power, int maxHealth){
        this.x = x;
        this.y = y;
        this.power = power;
        this.maxHealth = maxHealth;
        this.curHealth = maxHealth;
    }

    public boolean isDead(){
        return (curHealth == 0);
    }
    public int getPower(){
        return power;
    }

    public void decreaseHealth(int decreaseValue){
        curHealth -= decreaseValue;
        if (curHealth < 0){
            curHealth = 0;
        }
    }
}
