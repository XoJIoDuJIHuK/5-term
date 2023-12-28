package by.controller;

import by.dto.BasicResponseDTO;
import by.model.BaseEntity;
import by.model.Offer;
import by.model.User;
import by.services.OfferService;
import by.services.UserService;
import lombok.extern.slf4j.Slf4j;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Pointcut;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@Slf4j
@Controller
@RequestMapping(value = "/api/v1/admin")
public class AdminController {
    @Autowired
    private OfferService offerService;
    @Autowired
    private UserService userService;

    @Pointcut("execution(* by.controller.AdminController.*(..))")
    public void adminMethods() {}

    @After("adminMethods()")
    @Around("adminMethods()")
    public Object logMethod(ProceedingJoinPoint joinPoint) throws Throwable {
        log.info("Before method execution: {}", joinPoint.getSignature().toShortString());
        Object result = joinPoint.proceed();
        log.info("After method execution: {}", joinPoint.getSignature().toShortString());
        return result;
    }

    @PutMapping(value = "")
    public ResponseEntity<BasicResponseDTO> createOfferAsAdmin(@RequestParam Map<String, String> requestParams) {
        String subject;
        int cost;
        long tutorId;
        User tutor;
        try {
            subject = requestParams.get("subject");
            cost = Integer.parseInt(requestParams.get("cost"));
            tutorId = Long.parseLong(requestParams.get("tutorId"));
            tutor = userService.findById(tutorId);
        } catch (NullPointerException e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        log.info("Creating offer with subject: {}, cost: {}, tutorId: {}", subject, cost, tutorId);
        Offer offer = new Offer(subject, tutor, cost);
        offerService.create(offer);
        return new ResponseEntity<>(new BasicResponseDTO("xd"), HttpStatus.OK);
    }

    @PostMapping(value = "")
    public ResponseEntity<BasicResponseDTO> updateOfferAsAdmin(@RequestParam Map<String, String> requestParams) {
        long offerId;
        String subject;
        int cost;
        long tutorId;
        User tutor;
        try {
            offerId = Long.parseLong(requestParams.get("offerId"));
            subject = requestParams.get("subject");
            cost = Integer.parseInt(requestParams.get("cost"));
            tutorId = Long.parseLong(requestParams.get("tutorId"));
            tutor = userService.findById(tutorId);
        } catch (NullPointerException e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        Offer offer = new Offer(subject, tutor, cost);
        offer.setId(offerId);
        offerService.save(offer);
        return new ResponseEntity<>(new BasicResponseDTO("xd"), HttpStatus.OK);
    }

    @DeleteMapping(value = "")
    public ResponseEntity<BasicResponseDTO> deleteOfferAsAdmin(@RequestParam Map<String, String> requestParams) {
        long offerId;
        try {
            offerId = Long.parseLong(requestParams.get("offerId"));
        } catch (NullPointerException e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        offerService.deleteById(offerId);
        return new ResponseEntity<>(new BasicResponseDTO("xd"), HttpStatus.OK);
    }
}