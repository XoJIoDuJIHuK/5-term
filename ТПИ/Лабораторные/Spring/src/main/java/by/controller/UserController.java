package by.controller;

import by.dto.BasicResponseDTO;
import by.exceptions.AccountAuthException;
import by.model.Order;
import by.model.User;
import by.services.OrderService;
import by.services.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.sql.Time;
import java.util.Date;
import java.util.Map;

@Slf4j
@Controller
@RequestMapping(value = "/api/v1/user")
public class UserController {
    @Autowired
    private UserService userService;

    @Autowired
    private OrderService orderService;

    @GetMapping(value = "/isAdmin")
    public ResponseEntity<BasicResponseDTO> isAdmin(@RequestParam Map<String, String> mapParam) {
        Long userId = Long.parseLong(mapParam.get("userId"));
        log.debug(userId.toString());
        User user = userService.findById(userId);
        if (user != null) {
            boolean isAdmin = user.isIsadmin();
            return new ResponseEntity<>(new BasicResponseDTO(isAdmin ? "xd" : "not xd"),
                    HttpStatus.OK);
        }
        else throw new AccountAuthException("Account not found!");
    }

    @GetMapping(value = "getmyname")
    public ResponseEntity<BasicResponseDTO> getMyName(@RequestParam Map<String, String> requestParams) {
        Long userId = Long.parseLong(requestParams.get("id"));
        User user = userService.findById(userId);
        if (user == null) {
            return new ResponseEntity<>(new BasicResponseDTO("xd"), HttpStatus.NOT_ACCEPTABLE);
        }
        return new ResponseEntity<>(new BasicResponseDTO(user.getName()), HttpStatus.OK);
    }
}