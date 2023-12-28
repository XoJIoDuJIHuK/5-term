package com.example.lab01.pubSub.filter;

import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;

import javax.jms.Destination;
import javax.jms.JMSContext;
import javax.jms.JMSException;
import javax.jms.JMSProducer;

@SuppressWarnings("DuplicatedCode")
public class producer {
    public static void main(String[] args){
        ConnectionFactory factory= new ConnectionFactory();
        try(JMSContext context = factory.createContext("admin", "admin")){
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676,mq://127.0.0.1:7676");
            Destination topic  = context.createTopic("Filter");
            JMSProducer producer = context.createProducer();
            producer.setProperty("isWithProperty", "TRUE");
            producer.send(topic, "message with property");
        } catch (JMSException e) {
            System.out.println("ConnectionConfigurationError: " + e.getMessage());
        }
    }
}