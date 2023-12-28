package by.services;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.env.Environment;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.stereotype.Service;

import java.util.Objects;

@Service
public class MailService {
    @Value("${spring.mail.from}")
    private String mailFrom;

    private final JavaMailSender javaMailSender;

    private final Environment environment;

    public MailService(JavaMailSender javaMailSender, Environment environment) {
        this.javaMailSender = javaMailSender;
        this.environment = environment;
    }

    public void sendEmail(String to, String subject, String body) {
        SimpleMailMessage mailMessage = new SimpleMailMessage();
        mailMessage.setTo(to);
        mailMessage.setSubject(subject);
        mailMessage.setText(body);
        mailMessage.setFrom(mailFrom);

        javaMailSender.send(mailMessage);
    }
}
