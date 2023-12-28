package com.example.lab14_1;

import com.sun.messaging.ConnectionConfiguration;
import com.sun.messaging.ConnectionFactory;

import jakarta.jms.*;
import jakarta.servlet.*;
import jakarta.servlet.annotation.*;
import jakarta.servlet.http.*;
import java.io.IOException;
import java.io.PrintWriter;

@WebServlet(name = "helloServlet", value = "/hello-servlet")
public class HelloServlet extends HttpServlet {
    private String message;

    public void init() {
        message = "Hello World!";
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        response.setContentType("text/html");

        // Hello
        PrintWriter out = response.getWriter();
        out.println("<html><body>");
        out.println("<h1>" + message + "</h1>");
        out.println("</body></html>");
    }
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        PrintWriter writer = response.getWriter();
        ConnectionFactory factory = new com.sun.messaging.ConnectionFactory();
        String text = request.getParameter("text-input");

        if (text != null && text.equals("Belarusian")) {
            try{
                jakarta.jms.JMSContext context = factory.createContext("admin","admin");
                factory.setProperty(ConnectionConfiguration.imqAddressList, "mq://127.0.0.1:7676, mq://127.0.0.1:7676");

                Destination cardsQueue = context.createQueue("Servlet");
                JMSProducer producer = context.createProducer();

                Nation belarusian = new Nation(text, 10000000);
                producer.send(cardsQueue, belarusian.toString());

                System.out.println("Placed an information...");
            } catch (Exception e) {
                System.out.println(e.getMessage());;
            }

            response.sendRedirect("");
        } else {
            writer.println("<html><body>");
            writer.println("<h1>" + text + "</h1><br><hr>");
            writer.println("</body></html>");
        }
    }

    public void destroy() {
    }
}