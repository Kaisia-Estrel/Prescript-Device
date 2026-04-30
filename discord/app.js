// Require the necessary discord.js classes
const { Client, Events, GatewayIntentBits, Collection, REST, Routes } = require('discord.js');
const { token, clientId } = require('./config.json');

// Create a new client instance
const client = new Client({
  intents: [GatewayIntentBits.DirectMessages]
});

client.commands = new Collection();

const prescribe = require('./prescribe');
client.commands.set(prescribe.data.name, prescribe);

client.once(Events.ClientReady, (readyClient) => {
  console.log(`Ready! Logged in as ${readyClient.user.tag}`);
});

client.on(Events.InteractionCreate, async (interaction) => {
  const command = interaction.client.commands.get(interaction.commandName);
  if (!command) {
    console.error(`No command matching ${interaction.commandName} was found.`);
    return;
  }

  try {
    await command.execute(interaction);
  } catch (error) {
    console.error(error);
    if (interaction.replied || interaction.deferred) {
      await interaction.followUp({ content: 'There was an error while executing this command!', ephemeral: true });
    } else {
      await interaction.reply({ content: 'There was an error while executing this command!', ephemeral: true });
    }
  }
})

client.login(token);
