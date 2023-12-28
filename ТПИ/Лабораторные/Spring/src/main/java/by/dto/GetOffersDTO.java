package by.dto;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
public class GetOffersDTO {
    private Long userId = null;
    private Long offerId = null;
    public GetOffersDTO(Long offerId) {
        this.offerId = offerId;
    }
}
