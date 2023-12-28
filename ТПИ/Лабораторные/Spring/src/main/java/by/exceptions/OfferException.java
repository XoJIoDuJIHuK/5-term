package by.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(HttpStatus.BAD_REQUEST)
public class OfferException extends RuntimeException {

    public OfferException(String msg, Throwable t) {
        super(msg, t);
    }

    public OfferException(String msg) {
        super(msg);
    }
}