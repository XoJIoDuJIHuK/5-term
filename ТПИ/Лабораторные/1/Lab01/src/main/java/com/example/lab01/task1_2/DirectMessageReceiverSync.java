package com.example.lab01.task1_2;


import com.example.lab01.Nation;
import com.sun.messaging.ConnectionFactory;
import com.sun.messaging.ConnectionConfiguration;

import javax.jms.JMSException;
import javax.jms.JMSConsumer;
import javax.jms.Message;

public class DirectMessageReceiverSync {
    public static void main(String[] args) {
        ConnectionFactory factory = new ConnectionFactory();
        JMSConsumer consumer;

        try (var context = factory.createContext("admin","admin")){
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676,mq://127.0.0.1:7676");
            var messagesQueue = context.createQueue("Destination");
            consumer = context.createConsumer(messagesQueue);
            System.out.println("Listening to Destination...");

            while (true) {
                System.out.println();
                Message message = consumer.receive();
                System.out.println("--------Received message--------");
                var nation = message.getBody(Nation.class);
                System.out.println(message);
                System.out.println("--------Nation--------");
                System.out.println(nation);
            }
        } catch (JMSException e){
            System.out.println("Error: " + e.getMessage());
        }
    }
}
