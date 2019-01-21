# --custom=mytopo.py --topo=mytopo

from mininet.topo import Topo
class MyTopo( Topo ):
	"Simple topology example."
	
	def __init__( self ):
		"Create custom topo."
		"""
                   [h1]      [h2]      [h3]
                     \        |         /
                      \       |        /
             [h4]----[s1]----[s2]----[s3]----[h5]
                      /     /    \      \
                     /     /      \      \
                   [h6] [s4]      [s5]   [h7]
			/  \      /  \
		       /    \    /    \
		     [h8]   [h9][h10]  [h11]
		"""

		Topo.__init__(self)
		# add hosts
		h1 = self.addHost( 'h1' )	
		h2 = self.addHost( 'h2' )	
		h3 = self.addHost( 'h3' )	
		h4 = self.addHost( 'h4' )	
		h5 = self.addHost( 'h5' )	
		h6 = self.addHost( 'h6' )	
		h7 = self.addHost( 'h7' )	
		h8 = self.addHost( 'h8' )	
		h9 = self.addHost( 'h9' )	
		h10 = self.addHost( 'h10' )	
		h11 = self.addHost( 'h11' )	
		# add switches
		s1 = self.addSwitch( 's1' )
		s2 = self.addSwitch( 's2' )
		s3 = self.addSwitch( 's3' )
		s4 = self.addSwitch( 's4' )
		s5 = self.addSwitch( 's5' )
		# add links
		self.addLink( s1, s2 )
		self.addLink( s2, s3 )
		self.addLink( s2, s4 )
		self.addLink( s2, s5 )
		self.addLink( s1, h1 )
		self.addLink( s1, h4 )
		self.addLink( s1, h6 )
		self.addLink( s2, h2 )
		self.addLink( s3, h3 )
		self.addLink( s3, h5 )
		self.addLink( s3, h7 )
		self.addLink( s4, h8 )
		self.addLink( s4, h9 )
		self.addLink( s5, h10 )
		self.addLink( s5, h11 )
	
topos = { 'mytopo': ( lambda: MyTopo() ) }
