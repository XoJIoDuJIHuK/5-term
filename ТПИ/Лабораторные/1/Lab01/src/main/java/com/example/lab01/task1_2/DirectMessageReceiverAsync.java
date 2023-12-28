package com.example.lab01.task1_2;

import com.example.lab01.Nation;
import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;
import org.jetbrains.annotations.NotNull;

import javax.jms.JMSConsumer;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageListener;


public class DirectMessageReceiverAsync implements MessageListener {

    ConnectionFactory factory = new ConnectionFactory();
    JMSConsumer consumer;
    DirectMessageReceiverAsync() {
        try (var context = factory.createContext("admin","admin")){
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676,mq://127.0.0.1:7676");
            var messagesQueue = context.createQueue("Destination");
            consumer = context.createConsumer(messagesQueue);
            consumer.setMessageListener(this);

            System.out.println("Listening to Destination...");
            Thread.sleep(1234567890);
        } catch (JMSException | InterruptedException e){
            System.out.println("Error: " + e.getMessage());
        }
    }

    @Override
    public void onMessage(@NotNull Message message) {
        try {
            System.out.println();
            System.out.println("------Received message------");
            var nation = message.getBody(Nation.class);
            System.out.println(message);
            System.out.println("------Nation------");
            System.out.println(nation);
        }
        catch (JMSException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        new DirectMessageReceiverAsync();
    }
}
