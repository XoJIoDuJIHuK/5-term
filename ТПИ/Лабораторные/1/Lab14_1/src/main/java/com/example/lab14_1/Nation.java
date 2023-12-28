package com.example.lab14_1;

import java.io.Serializable;

public class Nation implements Serializable {
    private String name;
    private int number;

    public Nation() {}
    public Nation(String name, int number) {
        this.name = name;
        this.number = number;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    @Override public String toString() {
        return "Nation " + this.name + " " + this.number;
    }
}
