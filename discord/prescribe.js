const { Integration, ApplicationIntegrationType } = require('discord.js');
const { SlashCommandBuilder, InteractionContextType } = require('discord.js');
const fs = require('fs');
const { Worker } = require('worker_threads');

const worker = new Worker("./prescript-worker.js");

let id = 0;
let pending = new Map();

function enqueue(content, interaction) {
  const jobId = id++;

  const author = 
      (interaction.user.member?.nickname ??
      interaction.user.globalName ??
      interaction.user.username).substr(0,16);

  pending.set(jobId, interaction);
  worker.postMessage({ content, author, jobId });
}

worker.on('message', async ({jobId, result}) => {
  const interaction = pending.get(jobId);
  if (!interaction) return;
  pending.delete(jobId);

  const userId = interaction.user.id;
  if (result) {
    await interaction.followUp(`<@${userId}>, Prescript Completed`);
  } else {
    await interaction.followUp(`<@${userId}>, Prescript Finished`);
  }
});

module.exports = {
  data: new SlashCommandBuilder()
    .setName('prescribe')
    .setDescription('Assign a prescript')
    .setIntegrationTypes(ApplicationIntegrationType.UserInstall)
    .setContexts([InteractionContextType.PrivateChannel, InteractionContextType.BotDM, InteractionContextType.Guild])
    .addStringOption(option =>
      option.setName('content')
        .setDescription('Prescript Text')
        .setRequired(true))
  ,
  async execute(interaction) {
    const content = interaction.options.getString('content').substr(0,60);
    await interaction.reply("Prescript Given");
    enqueue(content, interaction);
  },
};
