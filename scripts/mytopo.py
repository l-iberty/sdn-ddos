# --custom=mytopo.py --topo=mytopo

from mininet.topo import Topo
class MyTopo( Topo ):
	"Simple topology example."
	
	def __init__( self ):
		"Create custom topo."
		"""
                        [h1]
                         |
                        [s1]
                       /    \
                      /      \
                    [s2]     [s3]
                    /  \     /   \
                  [h2] [h3] [h4] [h5]
		"""

		Topo.__init__(self)
		# add hosts
		h1 = self.addHost( 'h1' )	
		h2 = self.addHost( 'h2' )	
		h3 = self.addHost( 'h3' )	
		h4 = self.addHost( 'h4' )	
		h5 = self.addHost( 'h5' )	
		# add switches
		s1 = self.addSwitch( 's1' )
		s2 = self.addSwitch( 's2' )
		s3 = self.addSwitch( 's3' )
		# add links
		self.addLink( s1, s2 )
		self.addLink( s1, s3 )
		self.addLink( s1, h1 )
		self.addLink( s2, h2 )
		self.addLink( s2, h3 )
		self.addLink( s3, h4 )
		self.addLink( s3, h5 )
	
topos = { 'mytopo': ( lambda: MyTopo() ) }

