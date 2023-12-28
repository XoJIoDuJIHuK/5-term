package by.exceptions;

import lombok.Getter;
import org.springframework.http.HttpStatus;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ResponseStatus;

@Getter
@ResponseStatus(HttpStatus.BAD_REQUEST)
public class OfferValidationException extends RuntimeException
{
    private final BindingResult bindingResult;

    public OfferValidationException(BindingResult bindingResult) {
        this.bindingResult = bindingResult;
    }

}