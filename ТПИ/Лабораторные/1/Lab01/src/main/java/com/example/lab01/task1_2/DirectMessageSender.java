package com.example.lab01.task1_2;

import com.example.lab01.Nation;
import com.sun.messaging.ConnectionFactory;
import com.sun.messaging.ConnectionConfiguration;

import javax.jms.JMSException;
import javax.jms.JMSProducer;

public class DirectMessageSender {

    public static void main(String[] args) {

        ConnectionFactory factory = new ConnectionFactory();
        JMSProducer producer;

        try (var context = factory.createContext("admin","admin")){
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676,mq://127.0.0.1:7676");
            var messagesQueue = context.createQueue("Destination");
            producer = context.createProducer();
            var nation = new Nation("Belarusian", 10000000);
            var objectMessage =context.createObjectMessage(nation);

            producer.send(messagesQueue,objectMessage);
            System.out.println("Placed an information about card transaction to Destination");
        } catch (JMSException e){
            System.out.println("Error: " + e.getMessage());
        }
    }
}

