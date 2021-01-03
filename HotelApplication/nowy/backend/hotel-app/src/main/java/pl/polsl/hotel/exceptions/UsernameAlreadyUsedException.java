package pl.polsl.hotel.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.CONFLICT)
public class UsernameAlreadyUsedException extends RuntimeException {

    public UsernameAlreadyUsedException(String userName) {
        super("User with username '" + userName + "' already exists");
    }

}