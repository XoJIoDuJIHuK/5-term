package by;

import by.model.Offer;
import by.repository.OfferRepository;
import by.repository.OrderRepository;
import by.repository.UserRepository;
import org.hamcrest.Matchers;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;

import java.util.Arrays;
import java.util.List;

import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
class ApplicationTests {

    @MockBean
    OfferRepository offerRepository;

    @MockBean
    UserRepository userRepository;

    @MockBean
    OrderRepository orderRepository;

    @Autowired
    WebApplicationContext webApplicationContext;

    private MockMvc mockMvc;

    public void setUp() {
        mockMvc = MockMvcBuilders.webAppContextSetup(webApplicationContext).build();
    }

    @Test
    public void testGetOffersFromRepository() {
        List<Offer> offers = Arrays.asList(
                new Offer("maths", userRepository.getById(1L), 2),
                new Offer("biology", userRepository.getById(3L), 7654),
                new Offer("deutsch", userRepository.getById(3L), 321)
        );

        when(offerRepository.findAll()).thenReturn(offers);

        List<Offer> result = offerRepository.findAll();

        Assertions.assertEquals(result, offers);
    }

    @Test
    public void testGetOffers() throws Exception {
        setUp();
        List<Offer> offers = Arrays.asList(
                new Offer("maths", userRepository.getById(1L), 2),
                new Offer("biology", userRepository.getById(3L), 7654),
                new Offer("deutsch", userRepository.getById(3L), 321)
        );

        when(offerRepository.findAll()).thenReturn(offers);

        mockMvc.perform(MockMvcRequestBuilders.get("/api/v1/scooter/list"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$", Matchers.hasSize(2)));
    }
}