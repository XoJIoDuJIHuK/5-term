package by.dto;

import by.model.User;
import lombok.Getter;
import org.hibernate.validator.constraints.Length;
import javax.validation.constraints.NotBlank;

@Getter
public class AuthUserDTO {

    @NotBlank(message = "Account username cannot be empty")
    @Length(min = 3, max = 40, message = "Account username must be between 3 and 40 characters")
    private String login;

    @NotBlank(message = "Password cannot be empty")
    @Length(min = 3, max = 16, message = "Password length must be between 3 and 16 characters")
    private String password;

    private String email;

    public User ToUser() {
        return new User(this.login, this.password, this.email, false);
    }
}