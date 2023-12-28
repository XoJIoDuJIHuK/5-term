package by.security;

import by.model.User;
import by.security.jwt.JwtUser;
import by.services.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

@Service
public class JwtUserDetailsService implements UserDetailsService {
    private final UserService userService;

    final PasswordEncoder passwordEncoder;

    @Autowired
    public JwtUserDetailsService(UserService userService, PasswordEncoder passwordEncoder) {
        this.userService = userService;
        this.passwordEncoder = passwordEncoder;
    }

    @Override
    public JwtUser loadUserByUsername(String login) throws UsernameNotFoundException {
        User user = userService.findByLogin(login);
        if (user == null) {
            throw new UsernameNotFoundException("User with login: " + login + " not found");
        }
        return JwtUser.fromUserToJwtUser(user);
    }
}