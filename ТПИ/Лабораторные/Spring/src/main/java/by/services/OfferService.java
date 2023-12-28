package by.services;

import by.model.Offer;
import by.repository.OfferRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.List;

@Service
public class OfferService {
    final
    OfferRepository offerRepository;

    public OfferService(OfferRepository offerRepository) {
        this.offerRepository = offerRepository;
    }

    public List<Offer> getAll() {
        return offerRepository.findAll();
    }

    public Offer findById(Long id) {
        return offerRepository.findOfferById(id);
    }
    public void create(Offer offer) {
        offerRepository.save(offer);
    }
    public void delete(Offer offer) {
        offerRepository.delete(offer);
    }

    public void update(Offer offer) {
        offerRepository.save(offer);
    }

    public void save(Offer offer) {
        offerRepository.save(offer);
    }

    public void deleteById(long offerId) {
        offerRepository.deleteById(offerId);
    }
    public List<Offer> findBySubject(String subject) {
        return offerRepository.findBySubject(subject);
    }
}