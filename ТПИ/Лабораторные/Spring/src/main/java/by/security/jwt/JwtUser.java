package by.security.jwt;

import by.model.User;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import java.util.Collection;
import java.util.Collections;

public class JwtUser implements UserDetails {

    private String login;
    private String password;
    private Collection<? extends GrantedAuthority> authorities;

    public static JwtUser fromUserToJwtUser(User user) {
        JwtUser j = new JwtUser();
        j.login = user.getName();
        j.password = user.getPassword();
        j.authorities = Collections.singletonList(new SimpleGrantedAuthority(user.isIsadmin() ?
                "ROLE_ADMIN" : "ROLE_USER"));
        return j;
    }

    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        return authorities;
    }

    @Override
    public String getPassword() {
        return password;
    }

    @Override
    public String getUsername() {
        return login;
    }

    @Override
    public boolean isAccountNonExpired() {
        return true;
    }

    @Override
    public boolean isAccountNonLocked() {
        return true;
    }

    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }

    @Override
    public boolean isEnabled() {
        return true;
    }
}