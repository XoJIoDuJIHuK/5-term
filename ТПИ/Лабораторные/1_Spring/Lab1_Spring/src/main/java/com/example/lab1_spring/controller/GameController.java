package com.example.lab1_spring.controller;

import com.example.lab1_spring.forms.GameForm;
import com.example.lab1_spring.model.Game;

import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.springframework.ui.Model;
import org.springframework.web.servlet.ModelAndView;

import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;

@Slf4j
@Controller
public class GameController {
    private static List<Game> games = new ArrayList<>();
    static {
        games.add(new Game("War Thunder", "Gaijin Entertainment"));
        games.add(new Game("Hearts of Iron 4", "Paradox Development Studio"));
    }
    @Value("${welcome.message}")
    private String message;

    @Value("${error.message}")
    private String errorMessage;

    @GetMapping(value = {"/", "/index"})
    public ModelAndView index(Model model) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("index");
        model.addAttribute("message", message);
        log.info("/index was called");
        return modelAndView;
    }

    @GetMapping(value = {"/allgames"})
    public ModelAndView personList(Model model) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("gameslist");
        model.addAttribute("games", games);
        log.info("/allgames GET was called");
        return modelAndView;
    }

    @GetMapping(value = {"/addgame"})
    public  ModelAndView showAddGamePage(Model model) {
        ModelAndView modelAndView = new ModelAndView("addgame");
        GameForm bookForm = new GameForm();
        model.addAttribute("gameform", bookForm);
        log.info("/addgame GET was called");
        return modelAndView;
    }

    @PostMapping(value = {"/addgame"})
    public ModelAndView addGame(Model model, @ModelAttribute("gameform") GameForm gameForm) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("gameslist");
        String name = gameForm.getName();
        String developerName = gameForm.getDeveloperName();

        if (name != null && !name.isEmpty() //
                && developerName != null && !developerName.isEmpty()) {
            Game newGame = new Game(name, developerName);
            games.add(newGame);
            model.addAttribute("games", games);
            return modelAndView;
        }
        model.addAttribute("errorMessage", errorMessage);
        modelAndView.setViewName("addgame");
        log.info("/addgame POST was called");
        return modelAndView;
    }

    @GetMapping(value = {"/editgame"})
    public  ModelAndView showEditGamePage(Model model) {
        ModelAndView modelAndView = new ModelAndView("editgame");
        GameForm bookForm = new GameForm();
        model.addAttribute("gameform", bookForm);
        log.info("/editgame GET was called");
        return modelAndView;
    }

    @PostMapping(value = {"/editgame"})
    public ModelAndView editGame(Model model, @ModelAttribute("gameform") GameForm gameForm) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("gameslist");
        String name = gameForm.getName();
        String developerName = gameForm.getDeveloperName();

        if (name != null && !name.isEmpty() //
                && developerName != null && !developerName.isEmpty()) {

            games.stream()
                    .filter(game -> game.getName().equals(name))
                    .forEach(game -> game.setDeveloperName(developerName));

            model.addAttribute("games", games);
            return modelAndView;
        }
        model.addAttribute("errorMessage", errorMessage);
        modelAndView.setViewName("editgame");
        log.info("/editgame POST was called");
        return modelAndView;
    }

    @GetMapping(value = {"/deletegame"})
    public  ModelAndView showDeleteGamePage(Model model) {
        ModelAndView modelAndView = new ModelAndView("deletegame");
        GameForm bookForm = new GameForm();
        model.addAttribute("gameform", bookForm);
        log.info("/deletegame GET was called");
        return modelAndView;
    }

    @PostMapping(value = {"/deletegame"})
    public ModelAndView deleteGame(Model model, @ModelAttribute("gameform") GameForm gameForm) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("gameslist");
        String name = gameForm.getName();
        String developerName = gameForm.getDeveloperName();

        if (name != null && !name.isEmpty() //
                && developerName != null && !developerName.isEmpty()) {

            games = games.stream()
                    .filter(game -> (!game.getName().equals(name) &&
                            !game.getDeveloperName().equals(developerName)))
                    .collect(Collectors.toList());

            model.addAttribute("games", games);
            return modelAndView;
        }
        model.addAttribute("errorMessage", errorMessage);
        modelAndView.setViewName("editgame");
        log.info("/deletegame POST was called");
        return modelAndView;
    }
}
