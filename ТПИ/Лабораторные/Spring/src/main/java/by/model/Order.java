package by.model;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import javax.persistence.*;
import java.util.Optional;

@EqualsAndHashCode(callSuper = true)
@Data
@Entity
@NoArgsConstructor
@Table(name = "orders")
public class Order extends BaseEntity{

    @ManyToOne
    @JoinColumn(name = "user_id")
    private User user;

    @ManyToOne
    @JoinColumn(name = "offer_id")
    private Offer offer;

    @Column(name = "amount_of_hours")
    private int amountOfHours;

    @Column(name = "rating")
    private int rating = -1;

    public Order(User user, Offer offer, int amountOfHours) {
        this.user = user;
        this.offer = offer;
        this.amountOfHours = amountOfHours;
    }
}