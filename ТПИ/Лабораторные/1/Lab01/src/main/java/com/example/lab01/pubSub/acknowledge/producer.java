package com.example.lab01.pubSub.acknowledge;

import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;

import javax.jms.Destination;
import javax.jms.JMSContext;
import javax.jms.JMSException;

@SuppressWarnings("DuplicatedCode")
public class producer {
    public static void main(String[] args){
        ConnectionFactory factory= new ConnectionFactory();
        try(JMSContext context = factory.createContext("admin", "admin")){
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676,mq://127.0.0.1:7676");
            Destination topic  = context.createTopic("Acknowledge");
            context.createProducer().send(topic, "message");
            System.out.println("message sent");
            //Thread.sleep(100000000);
        } catch (JMSException e) {
            System.out.println("ConnectionConfigurationError: " + e.getMessage());
        }
    }
}