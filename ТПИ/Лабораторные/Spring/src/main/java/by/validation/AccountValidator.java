package by.validation;

import by.dto.AuthUserDTO;
import by.services.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.validation.Errors;
import org.springframework.validation.Validator;

@Component
public class AccountValidator implements Validator {

    @Autowired
    private UserService userService;

    @Override
    public boolean supports(Class<?> aClass) {
        return AuthUserDTO.class.equals(aClass);
    }

    @Override
    public void validate(Object o, Errors errors) {
        AuthUserDTO account = (AuthUserDTO) o;

        if(userService.findByLogin(account.getLogin()) != null) {
            errors.rejectValue("login", "", "This login is already in use");
        }
    }
}