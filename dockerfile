FROM szymon432/arm64-compiler-image:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y supervisor

WORKDIR /Small-Home-Hub

# Copy files from local disc to container
COPY Build/arm64/Release/SHH /Small-Home-Hub/
# COPY Build/arm64/Release/Archiver /Small-Home-Hub/
COPY Build/arm64/Release/Backend_Server /Small-Home-Hub/
COPY Build/arm64/Release/Communication_Server /Small-Home-Hub/
# COPY Build/arm64/Release/Events_Handler /Small-Home-Hub/
# COPY Build/arm64/Release/QuestDB_Runner /Small-Home-Hub/

# Make logs directory
RUN mkdir -p /Small-Home-Hub/logs

# Make database
RUN mkdir -p /Small-Home-Hub/Database
COPY Database/config.sql /Small-Home-Hub/Database
COPY makeDatabase.sh /Small-Home-Hub/Database
RUN /Small-Home-Hub/Database/makeDatabase.sh

# Install tzdata and configure timezone to Polska - simple but likes to make mess
RUN apt install -y tzdata && \
    ln -sf /usr/share/zoneinfo/Europe/Warsaw /etc/localtime && \
    echo "Europe/Warsaw" > /etc/timezone && \
    dpkg-reconfigure -f noninteractive tzdata

# Set environmental variable TZ
ENV TZ=Europe/Warsaw

# Configure Supervisor
COPY Miscellaneous/supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# QuestDB
# RUN apt install openjdk-17-jdk -y
# RUN echo "export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-arm64" >> ~/.bashrc
# ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-arm64
# RUN /bin/bash -c "source ~/.bashrc"
# RUN wget https://github.com/questdb/questdb/releases/download/8.1.1/questdb-8.1.1-no-jre-bin.tar.gz
# RUN tar -xvzf questdb-8.1.1-no-jre-bin.tar.gz
# RUN rm questdb-8.1.1-no-jre-bin.tar.gz
# RUN mv questdb-8.1.1-no-jre-bin questdb

# Start Supervisor as main process of contaier
CMD ["/usr/bin/supervisord"]

EXPOSE 9999/tcp
EXPOSE 9990/tcp
EXPOSE 9991/tcp