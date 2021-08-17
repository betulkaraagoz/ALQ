
#include "LinkedEventList.h"

LinkedEventList::LinkedEventList()
{
	eventCount = 0;
}


void LinkedEventList::add(std::unique_ptr<Event>&& event)
{
	add(std::move(event), false);
}

void LinkedEventList::add(std::unique_ptr<Event>&& event, bool grouping)
{
	Event* possiblyCompositeEvt = event.get();

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(possiblyCompositeEvt);

	if (compEvt == nullptr) {
		eventCount++;
	}
	else {
		eventCount += compEvt->size();
	}

	if (list.size() == 0) {
		list.push_back(std::move(event));
	}
	else {
		if (grouping && list.back() == event) {
			CompositeEvent* compositeTail = dynamic_cast<CompositeEvent*>(list.back().get());

			if (compositeTail) {
				if (compEvt == nullptr) {
					compositeTail->add(std::move(event));
				}
				else {
					//TODO
					compositeTail->add(compEvt);
				}
			}
			else {
				if (compEvt == nullptr) {
					auto newComposite = std::make_unique<CompositeEvent>(compEvt->getTimeStamp());
					newComposite.add(std::move(event));
					newComposite.add(std::move(list.back()));
					list.pop_back();
					list.push_back(std::move(newComposite));
				}
				else {
					compEvt->add(std::move(list.back()));
					list.pop_back();
					list.push_back(std::move(event));
				}
			}
		}
		else {
			list.push_back(std::move(event));
		}
	}


}

void LinkedEventList::addOrdered(std::unique_ptr<Event>&& event, bool grouping)
{
	Event* possiblyCompositeEvt = event.get();

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(possiblyCompositeEvt);

	if (compEvt == nullptr) {
		eventCount++;
	}
	else {
		eventCount += compEvt->size();
	}

	std::list<std::unique_ptr<Event>>::iterator it;
	for (it = list.begin(); it != list.end(); it++)
	{
		if (*event < **it || *event == **it) {
			if (grouping && *event == **it) {
				CompositeEvent* compositeCurr = dynamic_cast<CompositeEvent*>((*it).get());
				if (compositeCurr != nullptr) { //curr is composite
					if (compEvt == nullptr) {
						compositeCurr->add(std::move(event));
					}
					else { //event is composite too
						//TODO!!!
						**it = CompositeEvent(std::make_unique<CompositeEvent>(compositeCurr),
							std::make_unique<CompositeEvent>(compEvt));
					}
				}
				else {
					if (compEvt == nullptr) {
						auto newComposite = std::make_unique<CompositeEvent>(event->getTimeStamp());
						newComposite.add(std::move(**it));
						newComposite.add(std::move(event));
						*it = std::move(newComposite);

						//TODO curr elementi deðiþtirmek için ne yapmak lazým
						/*list.remove(it);
						list.insert(it, std::move(newComposite));*/
					}
					else {
						compEvt->addFirst(std::move(*it));
						//TODO: doðru mu?
						*it = std::make_unique<CompositeEvent>(compEvt);;
					}
				}
				
			}
			else {
				list.insert(it, std::move(event));
			}
			return;
		}
		//TODO
		/*else {
			cur = cur.prev in original 
		}
		*/
	}

	list.push_front(std::move(event));
}

void LinkedEventList::addFirst(std::unique_ptr<Event>&& event)
{
	Event* possiblyCompositeEvt = event.get();

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(possiblyCompositeEvt);

	if (compEvt == nullptr) {
		eventCount++;
	}
	else {
		eventCount += compEvt->size();
	}

	
	list.push_front(std::move(event));
}

std::unique_ptr<Event> LinkedEventList::getFirst()
{
	return std::move(list.front());
}

std::unique_ptr<Event> LinkedEventList::getLast()
{
	std::move(list.back()) ;
}

std::unique_ptr<Event> LinkedEventList::getFirstAtomic()
{
	assert(eventCount > 0);
	
	std::unique_ptr<Event> firstEvent = std::move(list.front());
	std::unique_ptr<Event> firstAtomic;

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(firstEvent.get());

	if (compEvt != nullptr) {
		firstAtomic = compEvt->getFirst();
		return firstAtomic;
	}

	return firstEvent;

}

std::unique_ptr<Event> LinkedEventList::removeFirstAtomic()
{
	assert(eventCount > 0);
	eventCount--;

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(list.front().get());

	std::unique_ptr<Event> returnValue;

	if (compEvt == nullptr) { //head is atomic
		returnValue = std::move(list.front());
		list.pop_front();
	}
	else {
		returnValue = compEvt->remove();
		if (compEvt->size() == 0) {
			list.pop_front();
		}
	}

	return returnValue;

}

std::unique_ptr<Event> LinkedEventList::removeFirst()
{
	assert(eventCount > 0);

	std::unique_ptr<Event> firstEvent = std::move(list.front());

	CompositeEvent* compEvt = dynamic_cast<CompositeEvent*>(firstEvent.get());

	if (!firstEvent->isComposite()) {
		eventCount--;
	}
	else {
		eventCount -= compEvt->size();
	}

	list.pop_front();
	return firstEvent;
}

void LinkedEventList::enqueueAndEmpties(LinkedEventList & other)
{
	list.splice(list.end(), other);
	eventCount += other.getEventCount();
}

int LinkedEventList::getEventCount()
{
	return eventCount;
}

bool LinkedEventList::isEmpty()
{
	if (list.size == 0)
		return true;
	return false;
}











































//void LinkedEventList::add(std::unique_ptr<Event> & event)
//{
//	add(event, false);
//}
//
//void LinkedEventList::add(std::unique_ptr<Event>& event, bool grouping)
//{
//	CompositeEvent compEvt = dynamic_cast<CompositeEvent&>(*event).asComposite();
//	
//	if (event->isComposite()) {
//		eventCount++;
//	}
//	else {
//		//TODO casting doðru mu
//		eventCount += compEvt.size();
//	}
//
//	if (head == nullptr) {
//		Node n = Node();
//		nodeCount++;
//		n.info = std::move(event);
//		//TODO!!!
//		head = std::make_unique<Node>(n);
//		tail = std::make_unique<Node>(n);
//	}else {
//		if (grouping && event == tail->info) {
//			CompositeEvent compositeTailEvt = dynamic_cast<CompositeEvent&>(*(tail->info)).asComposite();
//
//			if (tail->info->isComposite()) {
//				//tail is composite
//				if (!event->isComposite()) {
//					compositeTailEvt.add(event);
//				}
//				else {
//					tail->info = CompositeEvent(compositeTailEvt, compEvt);
//				}
//			}
//			else {
//				if (!event->isComposite()) {
//					CompositeEvent cEvt = CompositeEvent(event->getTimeStamp());
//					cEvt.add(tail->info);
//					cEvt.add(event);
//					tail->info = cEvt;
//				}
//				else {
//					compEvt.addFirst(tail->info);
//					tail->info = compEvt;
//				}
//			}
//		}
//		else {
//			Node n = Node();
//			nodeCount++;
//			n->infd = event;
//			tail->next = n;
//			n->prev = tail;
//			tail = n;
//
//		}
//	}
//}
//
//void LinkedEventList::addFirst(std::unique_ptr<Event>& event)
//{
//	nodeCount++;
//
//	if (event->isComposite()) {
//		eventCount++;
//	}
//	else {
//		//TODO casting doðru mu
//		eventCount += dynamic_cast<CompositeEvent&>(*event).asComposite().size();
//	}
//
//	if (head == nullptr) {
//		head = std::make_unique<Node>();
//		head->info = std::move(event);
//		tail = std::move(head);
//	}
//	else {
//		std::unique_ptr<Node> n = std::move(head);
//		head = std::make_unique<Node>();
//		head->info = std::move(event);
//		head->next = std::move(n);
//		n->prev = std::move(head);
//	}
//}
//
//LinkedEventList::Node::Node()
//{
//}
