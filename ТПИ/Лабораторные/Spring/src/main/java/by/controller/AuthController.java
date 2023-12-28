package by.controller;

import by.dto.BasicResponseDTO;
import by.model.User;
import by.security.jwt.JwtTokenProvider;
import by.dto.AuthUserDTO;
import by.exceptions.AccountAuthException;
import by.exceptions.AccountValidationException;
import by.services.UserService;
import by.validation.AccountValidator;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import javax.validation.Valid;
import java.util.HashMap;
import java.util.Map;

@Slf4j
@Controller
@RequestMapping(value = "/api/v1/auth/")
public class AuthController {

    @Autowired
    private AccountValidator accountValidator;
    @Autowired
    private UserService userService;
    @Autowired
    private JwtTokenProvider jwtTokenProvider;

    @RequestMapping(value = "/signup", method = RequestMethod.POST, produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<User> signUp(@Valid @RequestBody AuthUserDTO accountDetails, BindingResult errors)
    {
        accountValidator.validate(accountDetails, errors);
        try {
            if (errors.hasErrors()) {
                throw new AccountValidationException(errors);
            }
        } catch (AccountValidationException e) {
            return new ResponseEntity<>(new User(e.getMessage(), "", "", false), HttpStatus.BAD_REQUEST);
        }

        User user = accountDetails.ToUser();
        userService.signup(user);
        return new ResponseEntity<>(user, HttpStatus.CREATED);
    }

    @RequestMapping(value = "/signin", method = RequestMethod.POST, produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Map<Object, Object>> signIn(@RequestBody AuthUserDTO accountDetails) {
        String login = accountDetails.getLogin();
        String password = accountDetails.getPassword();
        User user;
        Map<Object, Object> response = new HashMap<>();
        try {
            user = userService.findByLoginAndPassword(login, password);
            if (user == null) {
                throw new NullPointerException();
            }
        } catch (NullPointerException e) {
            response.put("message", "user not found");
            return new ResponseEntity<>(response, HttpStatus.NOT_FOUND);
        }
        String token = jwtTokenProvider.createToken(login);
        response.put("token", token);
        response.put("userId", user.getId());
        return new ResponseEntity<>(response, HttpStatus.OK);
    }
}