package by.dto;

import by.model.Offer;
import by.model.User;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class OfferDTO {
    private Long id;
    private String subject;
    private String tutorName;
    private int cost;
    private boolean alreadyOrdered;

    public OfferDTO(Offer offer, boolean alreadyOrdered, User user) {
        this.id = offer.getId();
        this.subject = offer.getSubject();
        this.tutorName = offer.getTutor().getName();
        this.cost = offer.getCost();
        this.alreadyOrdered = alreadyOrdered;
    }
}
