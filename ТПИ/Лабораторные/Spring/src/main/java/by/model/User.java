package by.model;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

@EqualsAndHashCode(callSuper = true)
@Data
@Entity
@NoArgsConstructor
@Table(name = "Users")
public class User extends BaseEntity {

    @Column(name = "name", unique = true)
    private String name;

    @Column(name = "password")
    private String password;

    @Column(name = "isadmin")
    private boolean isadmin;

    @Column(name = "email")
    private String email;

    public User(String name, String password, String email, boolean isadmin) {
        this.name = name;
        this.password = password;
        this.email = email;
        this.isadmin = isadmin;
    }
}