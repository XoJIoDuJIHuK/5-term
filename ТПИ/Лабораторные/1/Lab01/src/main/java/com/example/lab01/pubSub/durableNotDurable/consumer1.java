package com.example.lab01.pubSub.durableNotDurable;

import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;

import javax.jms.*;

@SuppressWarnings("DuplicatedCode")
public class consumer1 implements MessageListener {
    private ConnectionFactory factory = new ConnectionFactory();
    private JMSConsumer consumer;

    consumer1() {
        try (JMSContext context = factory.createContext("admin", "admin")) {
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676, mq://127.0.0.1:7676");
            //create durable subscription
            String clientID = "Client1";
            context.setClientID(clientID);
            Topic topic = context.createTopic("PubSub");

            consumer = context.createDurableConsumer(topic, clientID);
            /*consumer = context.createConsumer(topic);*/
            consumer.setMessageListener(this);
            while (true) {
                Thread.sleep(1000);
            }
        } catch (JMSException | InterruptedException e) {
            System.out.println(e.getMessage());
        }
    }

    @Override
    public void onMessage(Message message) {
        try {
            System.out.println("recieved: "+ message.getBody(String.class));
        } catch (Exception e) {
            System.err.println("JMSException: " + e.toString());
        }
    }

    public static void main (String[] args){
        new consumer1();
    }
}
