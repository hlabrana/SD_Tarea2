# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import metadatos_pb2 as metadatos__pb2


class APIStub(object):
  # missing associated documentation comment in .proto file
  pass

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.DatosHaciatorre = channel.unary_unary(
        '/proto.API/DatosHaciatorre',
        request_serializer=metadatos__pb2.dataAvion.SerializeToString,
        response_deserializer=metadatos__pb2.responseTorre.FromString,
        )
    self.ComprobarDestino = channel.unary_unary(
        '/proto.API/ComprobarDestino',
        request_serializer=metadatos__pb2.dataDestino.SerializeToString,
        response_deserializer=metadatos__pb2.responseDestino.FromString,
        )
    self.SolicitarDespegue = channel.unary_unary(
        '/proto.API/SolicitarDespegue',
        request_serializer=metadatos__pb2.dataDespegue.SerializeToString,
        response_deserializer=metadatos__pb2.responseDespegue.FromString,
        )
    self.SolicitarAterrizaje = channel.unary_unary(
        '/proto.API/SolicitarAterrizaje',
        request_serializer=metadatos__pb2.dataAterrizaje.SerializeToString,
        response_deserializer=metadatos__pb2.responseAterrizaje.FromString,
        )
    self.AvisarDespegue = channel.unary_unary(
        '/proto.API/AvisarDespegue',
        request_serializer=metadatos__pb2.AvisoDespegue.SerializeToString,
        response_deserializer=metadatos__pb2.responseMensaje.FromString,
        )
    self.AvisarAterrizaje = channel.unary_unary(
        '/proto.API/AvisarAterrizaje',
        request_serializer=metadatos__pb2.AvisoDespegue.SerializeToString,
        response_deserializer=metadatos__pb2.responseMensaje.FromString,
        )
    self.Departures = channel.unary_unary(
        '/proto.API/Departures',
        request_serializer=metadatos__pb2.mensajes.SerializeToString,
        response_deserializer=metadatos__pb2.responseDepartures.FromString,
        )
    self.Arrivals = channel.unary_unary(
        '/proto.API/Arrivals',
        request_serializer=metadatos__pb2.mensajes.SerializeToString,
        response_deserializer=metadatos__pb2.responseArrivals.FromString,
        )


class APIServicer(object):
  # missing associated documentation comment in .proto file
  pass

  def DatosHaciatorre(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def ComprobarDestino(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SolicitarDespegue(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SolicitarAterrizaje(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def AvisarDespegue(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def AvisarAterrizaje(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def Departures(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def Arrivals(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_APIServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'DatosHaciatorre': grpc.unary_unary_rpc_method_handler(
          servicer.DatosHaciatorre,
          request_deserializer=metadatos__pb2.dataAvion.FromString,
          response_serializer=metadatos__pb2.responseTorre.SerializeToString,
      ),
      'ComprobarDestino': grpc.unary_unary_rpc_method_handler(
          servicer.ComprobarDestino,
          request_deserializer=metadatos__pb2.dataDestino.FromString,
          response_serializer=metadatos__pb2.responseDestino.SerializeToString,
      ),
      'SolicitarDespegue': grpc.unary_unary_rpc_method_handler(
          servicer.SolicitarDespegue,
          request_deserializer=metadatos__pb2.dataDespegue.FromString,
          response_serializer=metadatos__pb2.responseDespegue.SerializeToString,
      ),
      'SolicitarAterrizaje': grpc.unary_unary_rpc_method_handler(
          servicer.SolicitarAterrizaje,
          request_deserializer=metadatos__pb2.dataAterrizaje.FromString,
          response_serializer=metadatos__pb2.responseAterrizaje.SerializeToString,
      ),
      'AvisarDespegue': grpc.unary_unary_rpc_method_handler(
          servicer.AvisarDespegue,
          request_deserializer=metadatos__pb2.AvisoDespegue.FromString,
          response_serializer=metadatos__pb2.responseMensaje.SerializeToString,
      ),
      'AvisarAterrizaje': grpc.unary_unary_rpc_method_handler(
          servicer.AvisarAterrizaje,
          request_deserializer=metadatos__pb2.AvisoDespegue.FromString,
          response_serializer=metadatos__pb2.responseMensaje.SerializeToString,
      ),
      'Departures': grpc.unary_unary_rpc_method_handler(
          servicer.Departures,
          request_deserializer=metadatos__pb2.mensajes.FromString,
          response_serializer=metadatos__pb2.responseDepartures.SerializeToString,
      ),
      'Arrivals': grpc.unary_unary_rpc_method_handler(
          servicer.Arrivals,
          request_deserializer=metadatos__pb2.mensajes.FromString,
          response_serializer=metadatos__pb2.responseArrivals.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'proto.API', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
