package by.model;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import java.util.List;

import javax.persistence.*;

@EqualsAndHashCode(callSuper = true)
@Data
@Entity
@NoArgsConstructor
@Table(name = "offers")
public class Offer extends BaseEntity {

    @Column(name = "subject")
    private String subject;

    @ManyToOne
    @JoinColumn(name = "tutor")
    private User tutor;

    @Column(name = "cost")
    private int cost;

    public Offer(String subject, User tutor, int cost) {
        this.subject = subject;
        this.tutor = tutor;
        this.cost = cost;
    }
}