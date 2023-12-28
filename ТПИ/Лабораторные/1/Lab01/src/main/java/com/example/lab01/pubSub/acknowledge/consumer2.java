package com.example.lab01.pubSub.acknowledge;

import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;

import javax.jms.*;

@SuppressWarnings("DuplicatedCode")
public class consumer2 implements MessageListener {
    private ConnectionFactory factory = new ConnectionFactory();
    private JMSConsumer consumer;

    consumer2() {
        try (JMSContext context = factory.createContext("admin", "admin",
                JMSContext.CLIENT_ACKNOWLEDGE)) {// если перезапустить, сообщение снова придёт
            factory.setProperty(ConnectionConfiguration.imqAddressList,
                    "mq://127.0.0.1:7676, mq://127.0.0.1:7676");
            //create durable subscription

            String clientID = "Client2";
            context.setClientID(clientID);
            Topic topic = context.createTopic("Acknowledge");

            consumer = context.createDurableConsumer(topic, clientID);
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
            //message.acknowledge();
            System.out.println("recieved: "+ message.getBody(String.class));
        } catch (Exception e) {
            System.err.println("JMSException: " + e.toString());
        }
    }

    public static void main (String[] args){
        new consumer2();
    }
}
