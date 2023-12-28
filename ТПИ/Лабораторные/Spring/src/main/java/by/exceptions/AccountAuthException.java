package by.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(HttpStatus.BAD_REQUEST)
public class AccountAuthException extends RuntimeException {

    public AccountAuthException(String msg, Throwable t) {
        super(msg, t);
    }

    public AccountAuthException(String msg) {
        super(msg);
    }
}