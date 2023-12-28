package by.controller;

import by.aop.LogAnnotation;
import by.dto.*;
import by.exceptions.AccountAuthException;
import by.exceptions.OfferException;
import by.model.Order;
import by.model.Offer;
import by.model.User;
import by.security.jwt.JwtUser;
import by.services.MailService;
import by.services.OrderService;
import by.services.UserService;
import by.services.OfferService;
import io.swagger.annotations.ApiOperation;
import lombok.extern.java.Log;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.RequestEntity;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import javax.mail.MessagingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;

@Slf4j
@Controller
@RequestMapping(value = "/api/v1/offer")
public class OfferController {
    @Autowired
    private OfferService offerService;

    @Autowired
    private OrderService orderService;

    @Autowired
    private UserService userService;

    @Autowired
    private MailService mailService;

    @LogAnnotation
    @ApiOperation(value = "Get Offers", notes = "Retrieve a list of offers. If subject is provided, filtered list is returned")
    @GetMapping(value = "list")
    public ResponseEntity<List<OfferDTO>> getOffers(@RequestParam Map<String, String> requestParams) {
        User user;
        String subjectToFilter = "";
        try {
            long userId = Long.parseLong(requestParams.get("id"));
            subjectToFilter = requestParams.get("subject");
            user = userService.findById(userId);
            if (user == null) {
                throw new NullPointerException();
            }
        } catch (Exception e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(null, HttpStatus.UNAUTHORIZED);
        }
        List<Offer> offers;
        if (subjectToFilter == null || subjectToFilter.isEmpty()) {
            offers = offerService.getAll();
        } else {
            offers = offerService.findBySubject(subjectToFilter);
        }
        List<OfferDTO> offersDTO = new ArrayList<>();
        for (Offer offer: offers) {
            offersDTO.add(new OfferDTO(offer, orderService.alreadyOrdered(offer, user), user));
        }
        log.info("Get request for offer/list");
        return new ResponseEntity<>(offersDTO, HttpStatus.OK);
    }

    @ApiOperation(value = "Create New Offer", notes = "Creates new offer with data provided in query parameters")
    @PutMapping(value = "create")
    public ResponseEntity<BasicResponseDTO> createOffer(@RequestParam Map<String, String> requestParams) {
        try {
            String subject = requestParams.get("subject");
            Long tutorId = Long.valueOf(requestParams.get("userId"));
            int cost = Integer.parseInt(requestParams.get("cost"));
            User tutor = userService.findById(tutorId);
            Offer offer = new Offer(subject, tutor, cost);
            offerService.create(offer);
        } catch (NullPointerException e) {
            return new ResponseEntity<>(new BasicResponseDTO("lmao"), HttpStatus.I_AM_A_TEAPOT);
        }
        return new ResponseEntity<>(new BasicResponseDTO("ok"), HttpStatus.CREATED);
    }

    @ApiOperation(value = "Make order", notes = "Adds new order for specified user, offer and amount of hours")
    @PostMapping(value = "makeorder")
    public ResponseEntity<BasicResponseDTO> makeOrder(RequestEntity<OrderDTO> offerDTO) {
        Long offerId;
        Long userId;
        int amountOfHours;
        try {
            offerId = Objects.requireNonNull(offerDTO.getBody()).getOfferId();
            userId = Objects.requireNonNull(offerDTO.getBody()).getUserId();
            amountOfHours = Objects.requireNonNull(offerDTO.getBody()).getAmountOfHours();
        } catch (NullPointerException e) {
            return new ResponseEntity<>(new BasicResponseDTO("lmao error"), HttpStatus.NOT_FOUND);
        }
        User user = userService.findById(userId);
        Offer offer = offerService.findById(offerId);
        Order order = new Order(user, offer, amountOfHours);
        orderService.save(order);
        log.info("started sending mail");
        mailService.sendEmail(user.getEmail(), "You made order", "You made order for " + order.getOffer().getSubject());
        log.info("ended sending mail");
        return new ResponseEntity<>(new BasicResponseDTO("xd ok"), HttpStatus.OK);
    }

    @ApiOperation(value = "Delete Order", notes = "Deletes existing order based on user and offer")
    @DeleteMapping(value = "deleteorder")
    public ResponseEntity<BasicResponseDTO> deleteOrder(@RequestParam Map<String, String> requestParams) {
        long offerId;
        long userId;
        Offer offer;
        User user;
        try {
            offerId = Long.parseLong(requestParams.get("offerId"));
            userId = Long.parseLong(requestParams.get("userId"));
            offer = offerService.findById(offerId);
            user = userService.findById(userId);
            Order order = orderService.findByOfferAndUser(offer, user);
            if (offer == null || user == null) {
                throw new NullPointerException();
            }
            if (!order.getUser().getId().equals(user.getId())) {
                throw new AccountAuthException("this user is not supposed to delete this order");
            }
        } catch (Exception e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        mailService.sendEmail(user.getEmail(), "You canceled order", "You canceled order for " + offer.getSubject());
        log.info(String.valueOf(orderService.deleteByOfferAndUser(offer, user)));
        return new ResponseEntity<>(new BasicResponseDTO("ok"), HttpStatus.OK);
    }

    @ApiOperation(value = "Delete Offer", notes = "Deletes offer by its id")
    @DeleteMapping(value = "deleteoffer")
    public ResponseEntity<BasicResponseDTO> deleteOffer(@RequestParam Map<String, String> requestParams) {
        long offerId;
        long userId;
        Offer offer;
        User user;
        try {
            offerId = Long.parseLong(requestParams.get("offerId"));
            userId = Long.parseLong(requestParams.get("userId"));
            offer = offerService.findById(offerId);
            user = userService.findById(userId);
            if (offer == null || user == null) {
                throw new NullPointerException();
            }
            log.info(offer.getTutor().getId() + " " + user.getId());
            if (!offer.getTutor().getId().equals(user.getId())) {
                throw new AccountAuthException("this user is not supposed to delete this offer");
            }
        } catch (Exception e) {
            log.error(e.getMessage());
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        orderService.deleteByOffer(offer);
        offerService.delete(offer);
        return new ResponseEntity<>(new BasicResponseDTO("ok"), HttpStatus.OK);
    }

    @ApiOperation(value = "Rate Offer", notes = "Updates rating column value of specified order")
    @PostMapping(value = "rate")
    public ResponseEntity<BasicResponseDTO> rateOffer(@RequestParam Map<String, String> requestParams) {
        Offer offer;
        User user;
        long offerId = -1;
        long userId = -1;
        int newRating = -1;
        log.debug(requestParams.values().toString());
        try {
            offerId = Long.parseLong(requestParams.get("offerId"));
            userId = Long.parseLong(requestParams.get("userId"));
            newRating = Integer.parseInt(requestParams.get("rating"));
            if (newRating < 1 || newRating > 5) {
                throw new NumberFormatException("rating must be between 1 and 5, got " + newRating);
            }
            offer = offerService.findById(offerId);
            user = userService.findById(userId);
            if (offer == null || user == null) {
                throw new NullPointerException();
            }
            Order order = orderService.findByOfferAndUser(offer, user);
            order.setRating(newRating);
            orderService.save(order);
        } catch (Exception e) {
            log.error(e.getMessage() + offerId + " " + userId + " " + newRating);
            return new ResponseEntity<>(new BasicResponseDTO(e.getMessage()), HttpStatus.BAD_REQUEST);
        }
        return new ResponseEntity<>(new BasicResponseDTO("ok"), HttpStatus.OK);
    }
}