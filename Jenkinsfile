node {
  // Mark the code checkout 'stage'....
  stage 'Stage Checkout'

  // Checkout code from repository and update any submodules
  checkout scm
  sh 'git submodule update --init'  

  stage 'Stage Build'

  //branch name from Jenkins environment variables
  echo "My branch is: ${env.BRANCH_NAME}"

  def flavor = flavor(env.BRANCH_NAME)
  echo "Building flavor ${flavor}"

  sh "export TRAVIS_DEBIAN_DISTRIBUTION=sid; wget -O- http://travis.debian.net/script.sh | sh -"

  stage 'Stage Archive'
  //tell Jenkins to archive the debs
  step([$class: 'ArtifactArchiver', artifacts: '../*.deb', fingerprint: true])

  stage 'Stage Upload'
  // ...
}

@NonCPS
def flavor(branchName) {
  def matcher = (env.BRANCH_NAME =~ /([a-z_]+)/)
  assert matcher.matches()
  matcher[0][1]
}
