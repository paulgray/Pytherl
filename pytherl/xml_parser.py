from xml.dom.minidom import parse, parseString
import xml.dom

def build_repr(node):
	if node.nodeType == xml.dom.Node.ELEMENT_NODE:
		return (node.nodeName, node.attributes.items(), [build_repr(child) for child in node.childNodes])
	if node.nodeType == xml.dom.Node.TEXT_NODE:
		return node.data

dom1 = parseString('<myxml some_attr="some_val">Some data<empty/> some more data</myxml>')
print build_repr(dom1.childNodes[0])
