package by.config;

import by.model.User;
import by.repository.UserRepository;
import by.security.jwt.JwtTokenFilter;
import by.security.jwt.JwtTokenProvider;
import by.security.jwt.JwtUser;
import io.jsonwebtoken.Jwt;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.authentication.configuration.AuthenticationConfiguration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

import java.util.Arrays;

@Slf4j
@Configuration
@EnableWebSecurity
public class SecurityConfig extends WebSecurityConfigurerAdapter {

    private final JwtTokenProvider jwtTokenProvider;

    public SecurityConfig(JwtTokenProvider jwtTokenProvider) {
        this.jwtTokenProvider = jwtTokenProvider;
    }

    private static final String[] PUBLIC_ENDPOINTS = {"/api/v1/auth/**", "/signin", "/signup", "/", "/admin"};
    private static final String[] AUTHORIZED_ENDPOINTS = {"/api/v1/offer/**", "api/v1/user/isAdmin"};
    private static final String [] OFFER_ENDPOINT= {"/api/v1/**","/","/api/v1/admin/**","/signin","/signup","/js/offers.js",
            "/js/signIn.js","/js/signUp.js", "/admin","/js/admin.js"};

    @Bean
    public AuthenticationManager authenticationManager(AuthenticationConfiguration authenticationConfiguration) throws Exception {
        return super.authenticationManager();
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        JwtTokenFilter jwtTokenFilter = new JwtTokenFilter(jwtTokenProvider);
        http
                .cors().and()
                .csrf().disable()
                .httpBasic().disable()
                .sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS)
                .and()
                .authorizeRequests()
                    .antMatchers("/css/**","/img/**","/js/**").permitAll()
//                    .antMatchers(OFFER_ENDPOINT).permitAll()
                    .antMatchers(PUBLIC_ENDPOINTS).permitAll()
                    .antMatchers(AUTHORIZED_ENDPOINTS).authenticated()
                    //.antMatchers("/**").hasRole("ADMIN")
                .anyRequest().authenticated()
                .and()
                .addFilterBefore(jwtTokenFilter, UsernamePasswordAuthenticationFilter.class);
    }

    @Autowired
    public void configureGlobal(AuthenticationManagerBuilder auth, UserRepository userRepository) throws Exception {
        auth.userDetailsService(userDetailsService(userRepository)).passwordEncoder(passwordEncoder());
    }

    @Bean
    public UserDetailsService userDetailsService(UserRepository userRepository) {
        return username -> {
            User user = userRepository.findUserByName(username);
            log.error(user.getName());
            if (user != null) {
                JwtUser jwtUser = JwtUser.fromUserToJwtUser(user);
                log.error(Arrays.toString(jwtUser.getAuthorities().toArray()));
                return org.springframework.security.core.userdetails.User
                        .withUsername(username)
                        .password(user.getPassword())
                        .authorities(jwtUser.getAuthorities())
                        .accountExpired(false)
                        .accountLocked(false)
                        .credentialsExpired(false)
                        .disabled(false)
                        .build();
            } else {
                throw new UsernameNotFoundException("User not found with username: " + username);
            }
        };
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
}